#include "Logger.h"
#include <iostream>


namespace NLogger {
	FILE* g_pLogFile = NULL;
	bool g_bHasClosedLogFile = false;

	void OpenLogFile() {
        /*if (g_pLogFile) {
            CloseLogFile();
        }

        Msg(__FUNCTION__);
		//if (!g_bHasClosedLogFile) {
            //FString path = FPaths::ProjectModsDir() + "testLog24712.log";
            //char buffer[512];
            //FString::ToBlob(path, (uint8*)buffer, sizeof(buffer));
            const char* buffer = "C:/Users/Mica/source/Unreal/FPSBase/Mods/log.log";

            Fatal("Attempting to open log file at %s", buffer);
			fopen_s(&g_pLogFile, buffer, "w");
		//*/
	}

	void Log(const char* pszMsg) {
        const char* buffer = "C:/Users/latru/source/UE5-FPS-Base/FPSBase/Mods/log.log";
        fopen_s(&g_pLogFile, buffer, "a");

		if (g_pLogFile) {
			fwrite(pszMsg, 1, strnlen_s(pszMsg, 100000), g_pLogFile);
            fclose(g_pLogFile);
        }
        else {
            static bool bHasReportedLogFileError = false;
            //Fatal("Could not open log file for writing!");
            //Don't call Fatal here bcs Fatal calls this function, stackoverflow possible
            if (GEngine && !bHasReportedLogFileError) {
                GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, "Could not open log file for writing!");
                bHasReportedLogFileError = true;
            }
        }
        
        g_pLogFile = NULL;
	}

    void Log(const wchar_t* pszMsg) {
        const char* buffer = "C:/Users/latru/source/UE5-FPS-Base/FPSBase/Mods/log.log";
        fopen_s(&g_pLogFile, buffer, "a");

        if (g_pLogFile) {
            fwrite(pszMsg, sizeof(wchar_t), wcsnlen_s(pszMsg, 100000), g_pLogFile);
            fclose(g_pLogFile);
        }
        else {
            static bool bHasReportedLogFileError = false;
            //Fatal("Could not open log file for writing!");
            //Don't call Fatal here bcs Fatal calls this function, stackoverflow possible
            if (GEngine && !bHasReportedLogFileError) {
                GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, "Could not open log file for writing!");
                bHasReportedLogFileError = true;
            }
        }

        g_pLogFile = NULL;
    }

	void CloseLogFile() {
		/*g_bHasClosedLogFile = true;

		if (g_pLogFile) {
			fclose(g_pLogFile);
            g_pLogFile = NULL;
		}*/
	}

#define BUFFER_SIZE 2000000

    //-------------------------------------------------------------------------------------
    // Implementation functions
    //-------------------------------------------------------------------------------------

    char        buffer[BUFFER_SIZE];
    static void VMsgMaster(FColor c, ftime duration, const char* pszFormat,
        va_list args) {
        if (GEngine) {
            vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE - 1, pszFormat, args);
            FString str = buffer;
            GEngine->AddOnScreenDebugMessage(-1, duration, c, str);
        }
    }

    wchar_t     wbuffer[BUFFER_SIZE];
    static void VWMsgMaster(FColor c, ftime duration, const wchar_t* pszFormat,
        va_list args) {
        if (GEngine) {
            vswprintf_s(wbuffer, BUFFER_SIZE, pszFormat, args);
            FString str = wbuffer;
            GEngine->AddOnScreenDebugMessage(-1, duration, c, str);
        }
    }

    inline void MsgMaster(FColor c, ftime duration, const FString& str) {
        if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, duration, c, str); }
    }

    inline void VLogMaster(const char* pszFormat, va_list args) {
        vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE - 1, pszFormat, args);
        Log(buffer);
    }

    //-------------------------------------------------------------------------------------
    // Public interface
    //-------------------------------------------------------------------------------------

    void Message(const FString& str) { MsgMaster(FColor::Cyan, 15.0f, str); }
    void Message(const char* pszFormat, ...) {
        va_list args;
        va_start(args, pszFormat);
        VMsgMaster(FColor::Cyan, 5.0f, pszFormat, args);
        va_end(args);
    }
    void Message(FColor c, ftime duration, const char* pszFormat, ...) {
        va_list args;
        va_start(args, pszFormat);
        VMsgMaster(c, duration, pszFormat, args);
        va_end(args);
    }

    void Blurp(const FString& str) {
        if (GFrameCounter % 1000 == 0) {
            MsgMaster(FColor::Cyan, 0.2f, str);
        }
    }
    void Blurp(const char* pszFormat, ...) {
        va_list args;
        va_start(args, pszFormat);
        if (GFrameCounter % 1000 == 0) {
            VMsgMaster(FColor::Cyan, 0.3f, pszFormat, args);
        }
        va_end(args);
    }

    void Warning(const FString& str) { MsgMaster(FColor::Yellow, 5.0f, str); }
    void Warning(const char* pszFormat, ...) {
        va_list args;
        va_start(args, pszFormat);
        VMsgMaster(FColor::Yellow, 5.0f, pszFormat, args);
        va_end(args);
    }

    void Fatal(const FString& str) { MsgMaster(FColor::Red, 500.0f, str); }
    void Fatal(const char* pszFormat, ...) {
        va_list args;
        va_start(args, pszFormat);
        VMsgMaster(FColor::Red, 500.0f, pszFormat, args);
        VLogMaster(pszFormat, args);
        va_end(args);
    }
} // namespace NLogger

void Msg(const char* pszFormat, ...) {
    va_list args;
    va_start(args, pszFormat);
    NLogger::VMsgMaster(FColor::Cyan, 10.0f, pszFormat, args);
    va_end(args);
}
void Log(const char* pszFormat, ...) {
    va_list args;
    va_start(args, pszFormat);
    vsnprintf_s(NLogger::buffer, BUFFER_SIZE, BUFFER_SIZE - 1, pszFormat, args);
    NLogger::Log(NLogger::buffer);
    va_end(args);
}
void Log(const wchar_t* pszFormat, ...) {
    va_list args;
    va_start(args, pszFormat);
    vswprintf_s(NLogger::wbuffer, BUFFER_SIZE, pszFormat, args);
    NLogger::Log(NLogger::wbuffer);
    va_end(args);
}
void Msg(const wchar_t* pszFormat, ...) {
    va_list args;
    va_start(args, pszFormat);
    NLogger::VWMsgMaster(FColor::Cyan, 10.0f, pszFormat, args);
    va_end(args);
}