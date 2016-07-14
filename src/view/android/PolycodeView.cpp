/*
 Copyright (C) 2016 by Joachim Meyer
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "polycode/view/android/PolycodeView.h"
#include "polycode/core/PolyLogger.h"
#include "polycode/core/PolyRenderer.h"
#include "polycode/core/PolySoundManager.h"
#include "polycode/core/PolyOpenSLAudioInterface.h"
#include <android/looper.h>
#include <stdlib.h>

using namespace Polycode;

AndroidCore* core = NULL;
extern void android_main(struct startHelper* helper);

PolycodeView::PolycodeView(ANativeActivity* native, String title){
	windowTitle = title;
	
	native_activity = native;
	native_window = NULL;
	native_input = NULL;
	
// 	pthread_mutex_init(windowMutex, NULL);
//     pthread_cond_init(windowCond, NULL);
	
	native_activity->instance = this;
	native_activity->callbacks->onStart 				= onStart;
	native_activity->callbacks->onStop 					= onStop;
	native_activity->callbacks->onDestroy				= onDestroy;
	native_activity->callbacks->onNativeWindowCreated 	= onNativeWindowCreated;
	native_activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
	native_activity->callbacks->onResume 				= onResume;
	native_activity->callbacks->onPause 				= onPause;
	native_activity->callbacks->onWindowFocusChanged 	= onWindowFocusChanged;
	native_activity->callbacks->onInputQueueCreated		= onInputQueueCreated;
	native_activity->callbacks->onInputQueueDestroyed	= onInputQueueDestroyed;
	native_activity->callbacks->onSaveInstanceState		= onSaveInstanceState;
	native_activity->callbacks->onConfigurationChanged	= onConfiguartionChanged;
	
	native_config = AConfiguration_new();
	AConfiguration_fromAssetManager(native_config, native_activity->assetManager);
	
	sensorManager = ASensorManager_getInstance();
	
	const ASensor* accelerometer = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	const ASensor* gyroscope = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GYROSCOPE);
	const ASensor* orientation = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GAME_ROTATION_VECTOR);
	
	ALooper* looper = ALooper_forThread();
    if(looper == NULL){
		looper = ALooper_prepare(0);
	}
	
	sensorQueue = ASensorManager_createEventQueue(sensorManager, looper, ALOOPER_POLL_CALLBACK, sensorLoop, this);
	
	if(accelerometer){
		ASensorEventQueue_enableSensor(sensorQueue, accelerometer);
	}
	
	if(gyroscope){
		ASensorEventQueue_enableSensor(sensorQueue, gyroscope);
	}
	
	if(orientation){
 		ASensorEventQueue_enableSensor(sensorQueue, orientation);
	}
	gyroTimestamp = 0;
	WakeLock = NULL;
	
	firstWindowCreate = true;
	
}

PolycodeView::~PolycodeView(){}

bool PolycodeView::isInteractable(){
	return ((lifecycleFlags & APP_STATUS_INTERACTABLE) == APP_STATUS_INTERACTABLE);
}

void onStart(ANativeActivity* activity){
	Logger::log("onStart");
}

void onResume(ANativeActivity* activity){
	Logger::log("onResume");
	JNIWakeLock(activity, true);
	if(core){
		core->paused = false;
		OpenSLAudioInterface::queueCallback(NULL, core->getAudioInterface());
	}
	((PolycodeView*)activity->instance)->lifecycleFlags |= APP_STATUS_ACTIVE;
}

void onPause(ANativeActivity* activity){
	Logger::log("onPause");
	JNIWakeLock(activity, false);
	if(core)
		core->paused = true;
	((PolycodeView*)activity->instance)->lifecycleFlags &= ~APP_STATUS_ACTIVE;
}

void* onSaveInstanceState(ANativeActivity* activity, size_t *outSize){
	Logger::log("onSaveInstanceState");
}

void onStop(ANativeActivity* activity){
	Logger::log("onStop");
}

void onDestroy(ANativeActivity* activity){
	Logger::log("onDestroy");
	JavaVM* javaVM = activity->vm;
	JNIEnv* jniEnv;
	bool attached = false;
	
	if(javaVM->GetEnv((void**)&jniEnv, JNI_VERSION_1_6) == JNI_EDETACHED){
		JavaVMAttachArgs attachArgs;
		attachArgs.version = JNI_VERSION_1_6;
		attachArgs.name = "NativeThread";
		attachArgs.group = NULL;
		
		jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
		if(result == JNI_ERR){
			if(core)
				core->Shutdown();
			return;
		}
		attached = true;
	}
	jniEnv->DeleteGlobalRef(((PolycodeView*)activity->instance)->WakeLock);
	
	if(attached)
		javaVM->DetachCurrentThread();
	
	if(core)
		core->Shutdown();
}

void onWindowFocusChanged(ANativeActivity* activity, int hasFocus){
	Logger::log("onWindowFocusChanged");
	if(hasFocus){
		AndroidEvent event = AndroidEvent();
		event.eventGroup = AndroidEvent::SYSTEM_FOCUS_EVENT;
		event.eventCode = Core::EVENT_GAINED_FOCUS;
		if(core)
			core->handleSystemEvent(event);
		JNIAutoHideNavBar(activity);
		((PolycodeView*)activity->instance)->lifecycleFlags|=APP_STATUS_FOCUSED;
	}else{
		AndroidEvent event = AndroidEvent();
		event.eventGroup = AndroidEvent::SYSTEM_FOCUS_EVENT;
		event.eventCode = Core::EVENT_LOST_FOCUS;
		if(core)
			core->handleSystemEvent(event);
		((PolycodeView*)activity->instance)->lifecycleFlags&=~APP_STATUS_FOCUSED;
	}
}

void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow *window){
	Logger::log("onNativeWindowCreated");
	((PolycodeView*)activity->instance)->native_window = window;
	int width = ANativeWindow_getWidth(window);
	int height = ANativeWindow_getHeight(window);
	if (width > 0 && height > 0){
		((PolycodeView*)activity->instance)->lifecycleFlags|=APP_STATUS_HAS_REAL_SURFACE;
		if(core){
			if(((PolycodeView*)activity->instance)->firstWindowCreate){
				
				((PolycodeView*)activity->instance)->firstWindowCreate = false;
			}
			core->recreateContext = true;
			core->setDeviceSize(width, height);
			core->setVideoMode(core->getXRes(), core->getYRes(), true, false, core->getAALevel(), 0, true);
		}
	}
}

void onNativeWindowResized(ANativeActivity* activity, ANativeWindow *window){
	Logger::log("onNativeWindowResized");
}

void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow *window){
	Logger::log("onNativeWindowRedrawNeeded");
}

void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow *window){
	Logger::log("onNativeWindowDestroyed");
	((PolycodeView*)activity->instance)->native_window = NULL;
	if (core){
		core->recreateContext = true;
	}
	((PolycodeView*)activity->instance)->lifecycleFlags&=~APP_STATUS_HAS_REAL_SURFACE;
}

void onInputQueueCreated(ANativeActivity* activity, AInputQueue *queue){
	Logger::log("onInputQueueCreated");
	if (((PolycodeView*)activity->instance)->native_input != NULL) {
		AInputQueue_detachLooper(((PolycodeView*)activity->instance)->native_input);
	}
	((PolycodeView*)activity->instance)->native_input = queue;
	ALooper *loop = ALooper_prepare(0);
	AInputQueue_attachLooper(((PolycodeView*)activity->instance)->native_input, loop, ALOOPER_EVENT_INPUT, inputLoop, activity->instance);
}

void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue *queue){
	Logger::log("onInputQueueDestroyed");
	AInputQueue_detachLooper(((PolycodeView*)activity->instance)->native_input);
	((PolycodeView*)activity->instance)->native_input = NULL;
}

void onContentRectChanged(ANativeActivity* activity, const ARect *rect){
	Logger::log("onContentRectChanged");
}

void onConfiguartionChanged(ANativeActivity* activity){
	Logger::log("onConfiguartionChanged");
	core->getEGLMutex()->lock();
	AConfiguration_delete(((PolycodeView*)activity->instance)->native_config);
	((PolycodeView*)activity->instance)->native_config = AConfiguration_new();
	AConfiguration_fromAssetManager(((PolycodeView*)activity->instance)->native_config, activity->assetManager);
	core->getEGLMutex()->unlock();
}

void onLowMemory(ANativeActivity* activity){
	Logger::log("onLowMemory");
}

static int inputLoop(int fd, int events, void* data){
	AInputQueue* native_input = ((PolycodeView*)data)->native_input;
	AndroidEvent event;
	AInputEvent* aev;
	int type;
	int action;
	
	while(AInputQueue_hasEvents(native_input)>0){
		if(AInputQueue_getEvent(native_input, &aev)>=0){
			event = AndroidEvent();
			event.eventGroup = AndroidEvent::INPUT_EVENT;
			type = AInputEvent_getType(aev);
			if(type == AINPUT_EVENT_TYPE_KEY){
				event.eventTime = AKeyEvent_getEventTime(aev);
				int kC = AKeyEvent_getKeyCode(aev);
				if(core){
					event.keyCode = core->mapKey(kC);
					if(kC == AKEYCODE_VOLUME_DOWN){
						JNIVolumeControl(((PolycodeView*)data)->native_activity, false);
					} else if (kC == AKEYCODE_VOLUME_UP){
						JNIVolumeControl(((PolycodeView*)data)->native_activity, true);
					}
				}
				
				action = AKeyEvent_getAction(aev);
				if(action == AKEY_EVENT_ACTION_DOWN){
					event.eventCode = InputEvent::EVENT_KEYDOWN;
					
					AndroidEvent textEvent; 
					textEvent.text = JNIGetUnicodeChar(((PolycodeView*)data)->native_activity, AKEY_EVENT_ACTION_DOWN, kC, AKeyEvent_getMetaState(aev)); 
					textEvent.eventCode = InputEvent::EVENT_TEXTINPUT; 
					textEvent.eventGroup = AndroidEvent::INPUT_EVENT; 
					if(textEvent.text.length() > 0 && !(textEvent.text[0] < ' ' || textEvent.text[0] == 127) && textEvent.text[0] > 0) 
						core->handleSystemEvent(textEvent); 
				} else if (action == AKEY_EVENT_ACTION_UP){
					event.eventCode = InputEvent::EVENT_KEYUP;
				}
				core->handleSystemEvent(event);
			} else if(type == AINPUT_EVENT_TYPE_MOTION){
				event.eventTime = AMotionEvent_getEventTime(aev);
				int evSource = AInputEvent_getSource(aev);
				
				if (evSource & AINPUT_SOURCE_CLASS_POINTER){
					action = AMotionEvent_getAction(aev);
					int count = AMotionEvent_getPointerCount(aev);
					
					if (action == AMOTION_EVENT_ACTION_MOVE){
						AndroidEvent hEvent;
						int history = AMotionEvent_getHistorySize(aev);
						
						for (int j = 0; j<history; j++){
							hEvent = AndroidEvent();
							hEvent.eventTime = AMotionEvent_getHistoricalEventTime(aev, j);
							hEvent.eventGroup = AndroidEvent::INPUT_EVENT;
							
							std::vector<TouchInfo> touches;
							
							for (int i = 0; i <count; i++){
								TouchInfo ti;
								ti.position = Vector2(AMotionEvent_getHistoricalX(aev,i,j)/Services()->getRenderer()->getBackingResolutionScaleX(), AMotionEvent_getHistoricalY(aev,i,j)/Services()->getRenderer()->getBackingResolutionScaleY());
								if(evSource & AINPUT_SOURCE_TOUCHSCREEN){
									ti.type = TouchInfo::TYPE_TOUCH;
								} else {
									ti.type = TouchInfo::TYPE_PEN;
								}
								ti.id = AMotionEvent_getPointerId(aev, i);
								if(ti.id == (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) || AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT){
									hEvent.touch = ti;
								}
								touches.push_back(ti);
							}
							hEvent.eventCode = InputEvent::EVENT_TOUCHES_MOVED;
							hEvent.touches = touches;
							
							core->handleSystemEvent(hEvent);
						}
					} else {
						std::vector<TouchInfo> touches;
						for (int i = 0; i <count; i++){
							TouchInfo ti;
							//TODO: slight offset since retina implementation
							ti.position = Vector2(AMotionEvent_getX(aev,i)/(core->getBackingXRes()/core->getXRes()), AMotionEvent_getY(aev,i)/(core->getBackingYRes()/core->getYRes()));
							if(evSource & AINPUT_SOURCE_TOUCHSCREEN){
								ti.type = TouchInfo::TYPE_TOUCH;
							} else {
								ti.type = TouchInfo::TYPE_PEN;
							}
							ti.id = AMotionEvent_getPointerId(aev, i);
							if(ti.id == (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) || AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT){
								event.touch = ti;
							}
							touches.push_back(ti);
						}
						
						if (action == AMOTION_EVENT_ACTION_UP){
							event.eventCode = InputEvent::EVENT_TOUCHES_ENDED;
							event.touch = touches[0];
						} else if (action == AMOTION_EVENT_ACTION_DOWN){
							event.eventCode = InputEvent::EVENT_TOUCHES_BEGAN;
							event.touch = touches[0];
						} else if (action == AMOTION_EVENT_ACTION_POINTER_DOWN){
							event.eventCode = InputEvent::EVENT_TOUCHES_BEGAN;
						} else if (action == AMOTION_EVENT_ACTION_POINTER_UP){
							event.eventCode = InputEvent::EVENT_TOUCHES_ENDED;
						}
						core->handleSystemEvent(event);
					}
				} else if (evSource & AINPUT_SOURCE_CLASS_POSITION){
					
				}
			}
			AInputQueue_finishEvent(native_input, aev, 1);
		}
	}
	return 1;
}

static int sensorLoop(int fd, int events, void* data){
	ASensorEventQueue* queue = ((PolycodeView*)data)->sensorQueue;
	
	ASensorEvent event;
	while(ASensorEventQueue_getEvents(queue, &event, 1) > 0){
		float dT;
		switch(event.type){
			case ASENSOR_TYPE_ACCELEROMETER:
 				core->_setAcceleration(Vector3(event.acceleration.x,event.acceleration.y,event.acceleration.z));
				break;
			case ASENSOR_TYPE_GYROSCOPE:
				if(((PolycodeView*)data)->gyroTimestamp!=0){
					dT = (event.timestamp-((PolycodeView*)data)->gyroTimestamp)/1000000000.0f;
					core->_setGyroRotation(Vector3(event.vector.x*dT*TODEGREES, event.vector.y*dT*TODEGREES, event.vector.z*dT*TODEGREES));
				}
				((PolycodeView*)data)->gyroTimestamp = event.timestamp;
				break;
			case ASENSOR_TYPE_GAME_ROTATION_VECTOR:
				Quaternion q=Quaternion(event.data[3],event.data[0],event.data[1],event.data[2]);
				Vector3 vec = q.toEulerAngles();
 				core->deviceAttitude = q;
				break;
		}
	}
	
	return 1;
}

void ANativeActivity_onCreate(ANativeActivity* activity, void *savedState, size_t savedStateSize) {
	Logger::log("onCreate");
	
	struct startHelper* helper = new struct startHelper;
	helper->activity = activity;
	helper->running = 0;
	pthread_mutex_init(&helper->mutex, NULL);
    pthread_cond_init(&helper->cond, NULL);
	
	pthread_attr_t attr; 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&helper->thread, &attr, startApp, helper);

    // Wait for thread to start.
    pthread_mutex_lock(&helper->mutex);
    while (!helper->running) {
        pthread_cond_wait(&helper->cond, &helper->mutex);
    }
    pthread_mutex_unlock(&helper->mutex);
}

void* startApp(void* data){
	Logger::log("startApp");
	startHelper* helper = (startHelper*)data;
	
	android_main(helper);
	
    pthread_mutex_lock(&helper->mutex);
    if (((PolycodeView*)helper->activity->instance)->native_input != NULL) {
        AInputQueue_detachLooper(((PolycodeView*)helper->activity->instance)->native_input);
    }
    
    pthread_cond_broadcast(&helper->cond);
    pthread_mutex_unlock(&helper->mutex);
	delete helper;
}

//From http://stackoverflow.com/questions/21124051/receive-complete-android-unicode-input-in-c-c
int JNIGetUnicodeChar(ANativeActivity* native_activity, int eventType, int keyCode, int metaState){
	JavaVM* javaVM = native_activity->vm;
	JNIEnv* jniEnv;
	bool attached = false;
	
	if(javaVM->GetEnv((void**)&jniEnv, JNI_VERSION_1_6) ==JNI_EDETACHED){
		JavaVMAttachArgs attachArgs;
		attachArgs.version = JNI_VERSION_1_6;
		attachArgs.name = "NativeThread";
		attachArgs.group = NULL;
		
		jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
		if(result == JNI_ERR){
			return 0;
		}
		attached = true;
	}
	
	jclass class_key_event = jniEnv->FindClass("android/view/KeyEvent");
	int unicodeKey;

	if(metaState == 0){
		jmethodID method_get_unicode_char = jniEnv->GetMethodID(class_key_event, "getUnicodeChar", "()I");
		jmethodID eventConstructor = jniEnv->GetMethodID(class_key_event, "<init>", "(II)V");
		jobject eventObj = jniEnv->NewObject(class_key_event, eventConstructor, eventType, keyCode);

		unicodeKey = jniEnv->CallIntMethod(eventObj, method_get_unicode_char);
	}else{
		jmethodID method_get_unicode_char = jniEnv->GetMethodID(class_key_event, "getUnicodeChar", "(I)I");
		jmethodID eventConstructor = jniEnv->GetMethodID(class_key_event, "<init>", "(II)V");
		jobject eventObj = jniEnv->NewObject(class_key_event, eventConstructor, eventType, keyCode);

		unicodeKey = jniEnv->CallIntMethod(eventObj, method_get_unicode_char, metaState);
	}
	
	if(attached)
		javaVM->DetachCurrentThread();

	return unicodeKey;
}

//From: http://www.gamedev.net/topic/674511-auto-hide-nav-bar-on-android-app-ndk/#entry5270901
void JNIAutoHideNavBar(ANativeActivity* native_activity){
	JavaVM* javaVM = native_activity->vm;
	JNIEnv* jniEnv;
	bool attached = false;
	
	if(javaVM->GetEnv((void**)&jniEnv, JNI_VERSION_1_6) ==JNI_EDETACHED){
		JavaVMAttachArgs attachArgs;
		attachArgs.version = JNI_VERSION_1_6;
		attachArgs.name = "NativeThread";
		attachArgs.group = NULL;
		
		jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
		if(result == JNI_ERR){
			return;
		}
		attached = true;
	}

	jclass activityClass = jniEnv->FindClass("android/app/NativeActivity");
	jmethodID getWindow = jniEnv->GetMethodID(activityClass, "getWindow", "()Landroid/view/Window;");

	jclass windowClass = jniEnv->FindClass("android/view/Window");
	jmethodID getDecorView = jniEnv->GetMethodID(windowClass, "getDecorView", "()Landroid/view/View;");

	jclass viewClass = jniEnv->FindClass("android/view/View");
	jmethodID setSystemUiVisibility = jniEnv->GetMethodID(viewClass, "setSystemUiVisibility", "(I)V");

	jobject window = jniEnv->CallObjectMethod(native_activity->clazz, getWindow);

	jobject decorView = jniEnv->CallObjectMethod(window, getDecorView);

	jfieldID flagFullscreenID = jniEnv->GetStaticFieldID(viewClass, "SYSTEM_UI_FLAG_FULLSCREEN", "I");
	jfieldID flagHideNavigationID = jniEnv->GetStaticFieldID(viewClass, "SYSTEM_UI_FLAG_HIDE_NAVIGATION", "I");
	jfieldID flagImmersiveStickyID = jniEnv->GetStaticFieldID(viewClass, "SYSTEM_UI_FLAG_IMMERSIVE_STICKY", "I");

	int flagFullscreen = jniEnv->GetStaticIntField(viewClass, flagFullscreenID);
	int flagHideNavigation = jniEnv->GetStaticIntField(viewClass, flagHideNavigationID);
	int flagImmersiveSticky = jniEnv->GetStaticIntField(viewClass, flagImmersiveStickyID);

	int flag = flagFullscreen | flagHideNavigation | flagImmersiveSticky;

	jniEnv->CallVoidMethod(decorView, setSystemUiVisibility, flag);

	if(attached)
		javaVM->DetachCurrentThread();
}

void JNIWakeLock(ANativeActivity* native_activity, bool acquire){
	JavaVM* javaVM = native_activity->vm;
	JNIEnv* jniEnv;
	bool attached = false;
	
	if(javaVM->GetEnv((void**)&jniEnv, JNI_VERSION_1_6) ==JNI_EDETACHED){
		JavaVMAttachArgs attachArgs;
		attachArgs.version = JNI_VERSION_1_6;
		attachArgs.name = "NativeThread";
		attachArgs.group = NULL;
		
		jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
		if(result == JNI_ERR){
			return;
		}
		attached = true;
	}

	jobject WakeLock = ((PolycodeView*)native_activity->instance)->WakeLock;
	
	if(!WakeLock){
		jclass classNativeActivity = jniEnv->FindClass("android/app/NativeActivity");
		jclass classPowerManager = jniEnv->FindClass("android/os/PowerManager");
		
		jmethodID getSystemServiceID = jniEnv->GetMethodID(classNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
		jstring POWER_SERVICE = jniEnv->NewStringUTF("power");
		jobject PowerManager = jniEnv->CallObjectMethod(native_activity->clazz, getSystemServiceID, POWER_SERVICE);
		
		jmethodID newWakeLockID = jniEnv->GetMethodID(classPowerManager, "newWakeLock", "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;");
		int SCREEN_BRIGHT_WAKE_LOCK = 0x0000000a;
		long int ON_AFTER_RELEASE = 0x20000000;
		jstring jWakeLockTag = jniEnv->NewStringUTF("Polycode");

		WakeLock = jniEnv->CallObjectMethod(PowerManager, newWakeLockID, SCREEN_BRIGHT_WAKE_LOCK | ON_AFTER_RELEASE, jWakeLockTag);
		
		jniEnv->DeleteLocalRef(jWakeLockTag);
		jniEnv->DeleteLocalRef(POWER_SERVICE);
		
		((PolycodeView*)native_activity->instance)->WakeLock = jniEnv->NewGlobalRef(WakeLock);
	}
	
	jclass wakeLock = jniEnv->FindClass("android/os/PowerManager$WakeLock");

	if (acquire){
		jmethodID acquireWakeLockID = jniEnv->GetMethodID(wakeLock, "acquire", "()V");
		jniEnv->CallVoidMethod(WakeLock, acquireWakeLockID);
	} else {
		jmethodID isHeldWakeLockID = jniEnv->GetMethodID(wakeLock, "isHeld", "()Z");
		if(jniEnv->CallBooleanMethod(WakeLock, isHeldWakeLockID)){
			jmethodID releaseWakeLockID = jniEnv->GetMethodID(wakeLock, "release", "()V");
			jniEnv->CallVoidMethod(WakeLock, releaseWakeLockID);
		}
	}
	
	if(attached)
		javaVM->DetachCurrentThread();
}

void JNIVolumeControl(ANativeActivity* native_activity, bool up){
	JavaVM* javaVM = native_activity->vm;
	JNIEnv* jniEnv;
	bool attached = false;
	
	if(javaVM->GetEnv((void**)&jniEnv, JNI_VERSION_1_6) ==JNI_EDETACHED){
		JavaVMAttachArgs attachArgs;
		attachArgs.version = JNI_VERSION_1_6;
		attachArgs.name = "NativeThread";
		attachArgs.group = NULL;
		
		jint result = javaVM->AttachCurrentThread(&jniEnv, &attachArgs);
		if(result == JNI_ERR){
			return;
		}
		attached = true;
	}

	jclass classNativeActivity = jniEnv->FindClass("android/app/NativeActivity");
	jclass classAudioManager = jniEnv->FindClass("android/media/AudioManager");
	
	jclass ClassContext = jniEnv->FindClass("android/content/Context");
	jfieldID FieldAUDIO_SERVICE = jniEnv->GetStaticFieldID(ClassContext, "AUDIO_SERVICE", "Ljava/lang/String;");
	jobject AUDIO_SERVICE = jniEnv->GetStaticObjectField(ClassContext, FieldAUDIO_SERVICE);
	
	jmethodID getSystemServiceID = jniEnv->GetMethodID(classNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	jobject AudioManager = jniEnv->CallObjectMethod(native_activity->clazz, getSystemServiceID, AUDIO_SERVICE);
	
	int ADJUST_RAISE = 1;
	int ADJUST_LOWER = -1;
	jmethodID adjustVolumeID = jniEnv->GetMethodID(classAudioManager, "adjustVolume", "(II)V");
	
	if (up){
		jniEnv->CallVoidMethod(AudioManager, adjustVolumeID, ADJUST_RAISE, 0);
	} else {
		jniEnv->CallVoidMethod(AudioManager, adjustVolumeID, ADJUST_LOWER, 0);
	}
	
	if(attached)
		javaVM->DetachCurrentThread();
}