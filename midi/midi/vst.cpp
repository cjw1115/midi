#include "vst.h"
#include <map>
#include <string>
#include <memory>
#include <windows.h>
#include <mutex>
#include <iostream>

#include "vst/aeffect.h"
#include "vst/aeffectx.h"

typedef AEffect * (*PluginEntryProc)(audioMasterCallback audioMaster);



struct vst
{
	AEffect * effect;
	HMODULE module = nullptr;
	HWND editor = nullptr;
	VstMidiEvent events[256];
	uint count;
};

typedef std::shared_ptr<vst> vst_ptr;
std::map<std::string, vst_ptr> vsts;

std::mutex mutex;

// ---------------------------------------------------------------------------------------- -
static VstIntPtr VSTCALLBACK HostCallback(AEffect *effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void *ptr, float opt) {
	VstIntPtr result = 0;

	switch (opcode) {
	case audioMasterVersion:
		result = kVstVersion;
		break;

	case audioMasterCurrentId:       ///< [return value]: current unique identifier on shell plug-in  @see AudioEffect::getCurrentUniqueId
		break;

	case audioMasterIdle:            ///< no arguments  @see AudioEffect::masterIdle
		break;

	case DECLARE_VST_DEPRECATED(audioMasterWantMidi):
		return 1;
		break;

	case audioMasterGetTime:
		break;

	case audioMasterProcessEvents:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterIOChanged:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case DECLARE_VST_DEPRECATED(audioMasterNeedIdle):
		return 1;
		break;

	case audioMasterSizeWindow:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterGetSampleRate:
		//switch (config_get_current_output_type()) {
		//case OUTPUT_TYPE_WASAPI:
		//	result = wasapi_get_samplerate();
		//	break;

		//default:
		//	result = 44100;
		//	break;
		//}
		result = 44100;
		break;

	case audioMasterGetBlockSize:
		result = 32;
		break;

	case audioMasterGetInputLatency:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterGetOutputLatency:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;


	case audioMasterGetCurrentProcessLevel:
		/*if (export_rendering()) {
			return kVstProcessLevelOffline;
			}
			else {
			return kVstProcessLevelRealtime;
			}*/
		return kVstProcessLevelRealtime;
		break;

	case audioMasterGetAutomationState:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterOfflineStart:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterOfflineRead:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterOfflineWrite:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterOfflineGetCurrentPass:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterOfflineGetCurrentMetaPass:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterGetVendorString:
		if (ptr) {
			strcpy((char *)ptr, "");
			return 1;
		}
		break;

	case audioMasterGetProductString:
		if (ptr) {
			strcpy((char *)ptr, "");
			return 1;
		}
		break;

	case audioMasterGetVendorVersion:
		return 1;
		break;

	case audioMasterVendorSpecific:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterCanDo:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterGetLanguage:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterGetDirectory:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterUpdateDisplay:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterBeginEdit:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterEndEdit:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterOpenFileSelector:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	case audioMasterCloseFileSelector:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;

	default:
		printf("PLUG> HostCallback (opcode %d)\n index = %d, value = %p, ptr = %p, opt = %f\n", opcode, index, FromVstPtr<void>(value), ptr, opt);
		break;
	}

	return result;
}


uint load_vst_plugin(const char * filename)
{
	std::unique_lock<std::mutex> lock(mutex);
	
	if (vsts[filename] != nullptr)
	{
		return 0;
	}
	auto ptr = vst_ptr(new vst);

	ptr->module = LoadLibrary(filename);
	if (ptr->module == nullptr)
	{
		return -1;
	}

	////// get effect constructor.
	//PluginEntryProc entry = nullptr;
	//entry = (PluginEntryProc)GetProcAddress((HMODULE)ptr->module, "VSTPluginMain");
	//if (entry == nullptr)
	//	entry = (PluginEntryProc)GetProcAddress((HMODULE)ptr->module, "main");
	//if (entry == nullptr)
	//	return -1;

	//ptr->effect = entry(HostCallback);

	//// create effect instance
	//effect = mainProc(HostCallback);

	//if (effect && (effect->magic != kEffectMagic))
	//	effect = NULL;

	//if (effect == NULL)
	//	return NULL;

	//// open effect
	//effect->dispatcher(effect, effOpen, 0, NULL, 0, 0);
	//effect->dispatcher(effect, effBeginSetProgram, 0, NULL, 0, 0);
	//effect->dispatcher(effect, effSetProgram, 0, 0, 0, 0);
	//effect->dispatcher(effect, effEndSetProgram, 0, NULL, 0, 0);

	vsts[filename] = ptr;
	return 0;
}

uint unload_vst_plugin(const char * filename)
{
	std::unique_lock<std::mutex> lock(mutex);

	auto iter = vsts.find(filename);
	if (iter == vsts.end())
		return 0;
	
	auto ptr = iter->second;
	if (ptr->editor)
	{
		DestroyWindow(ptr->editor);
		ptr->editor = nullptr;
	}

	if (ptr->effect)
	{
		ptr->effect->dispatcher(ptr->effect, effClose, 0, NULL, 0, 0);
		ptr->effect = nullptr;
	}

	if (ptr->module)
	{
		FreeLibrary(ptr->module);
		ptr->module = nullptr;
	}
	vsts.erase(iter);
	return 0;
}