#include "pch.h"
#include "Utility.h"

//======================================================================================================================================================

std::optional<std::filesystem::path> open_file(sf::RenderWindow& window) {
    static const auto directory = std::filesystem::current_path().string();

    char filename[MAX_PATH];
    ZeroMemory(&filename, sizeof(filename));


    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));


    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = window.getSystemHandle();
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Select File";
    ofn.lpstrInitialDir = directory.c_str();
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn)) {
        return { filename };
    }
    else {
        switch (CommDlgExtendedError()) {
            case CDERR_DIALOGFAILURE:   LOG_ERROR("CDERR_DIALOGFAILURE");   break;
            case CDERR_FINDRESFAILURE:  LOG_ERROR("CDERR_FINDRESFAILURE");  break;
            case CDERR_INITIALIZATION:  LOG_ERROR("CDERR_INITIALIZATION");  break;
            case CDERR_LOADRESFAILURE:  LOG_ERROR("CDERR_LOADRESFAILURE");  break;
            case CDERR_LOADSTRFAILURE:  LOG_ERROR("CDERR_LOADSTRFAILURE");  break;
            case CDERR_LOCKRESFAILURE:  LOG_ERROR("CDERR_LOCKRESFAILURE");  break;
            case CDERR_MEMALLOCFAILURE: LOG_ERROR("CDERR_MEMALLOCFAILURE"); break;
            case CDERR_MEMLOCKFAILURE:  LOG_ERROR("CDERR_MEMLOCKFAILURE");  break;
            case CDERR_NOHINSTANCE:     LOG_ERROR("CDERR_NOHINSTANCE");     break;
            case CDERR_NOHOOK:          LOG_ERROR("CDERR_NOHOOK");          break;
            case CDERR_NOTEMPLATE:      LOG_ERROR("CDERR_NOTEMPLATE");      break;
            case CDERR_STRUCTSIZE:      LOG_ERROR("CDERR_STRUCTSIZE");      break;
            case FNERR_BUFFERTOOSMALL:  LOG_ERROR("FNERR_BUFFERTOOSMALL");  break;
            case FNERR_INVALIDFILENAME: LOG_ERROR("FNERR_INVALIDFILENAME"); break;
            case FNERR_SUBCLASSFAILURE: LOG_ERROR("FNERR_SUBCLASSFAILURE"); break;
            default:                    LOG_TRACE("File browser closed.");  break;
        }
        return {};
    }
}

//======================================================================================================================================================

void run_file(const std::string& file_path) {
    SHELLEXECUTEINFOA seia;
    ZeroMemory(&seia, sizeof(seia));

    seia.cbSize = sizeof(seia);
    seia.lpFile = file_path.c_str();
    seia.nShow = SW_SHOW;

    if (!ShellExecuteExA(&seia)) {
        LOG_ERROR("Failed to open file [{}], with error: {}.", file_path, GetLastError());
    }
}

//======================================================================================================================================================