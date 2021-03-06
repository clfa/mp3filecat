#include "pch.h"
#include <afx.h>
#include <iostream>
#include "mp3file.h"
#include <Shlwapi.h>
#include <atlconv.h>
#include <stdlib.h>

#pragma comment(lib, "Shlwapi.lib")

Mp3File file;

// 处理MP3文件方式，默认复制
enum EmProcMp3FileType {
    EmProcMp3FileType_Copy = 0,// 复制方式
    EmProcMp3FileType_Move,    // 剪切方式
};

void GetModuleFolderPath( LPSTR lpBuff, HINSTANCE hInstance )
{
    ::GetModuleFileNameA( hInstance, lpBuff, MAX_PATH / sizeof( char ) );
    for ( int i = MAX_PATH - 1; i >= 0; i-- )
    {
        if ( *(lpBuff + i) != '\\' )
        {
            *(lpBuff + i) = 0;
        }
        else {
            break;
        }
    }
}

int ProcMp3File( const char* pMp3, EmProcMp3FileType type = EmProcMp3FileType_Copy )
{
    if ( NULL == pMp3 )
        return 1;

    //char szModule[MAX_PATH] = { 0 };
    //GetModuleFolderPath( szModule, NULL );

    //char szSrc[MAX_PATH] = { 0 };
    //strcpy( szSrc, szModule );
    //strcat( szSrc, pMp3 );

    if ( file.Open( pMp3 ) ) {
        char szDstDir[MAX_PATH] = { 0 };
        char szDrive[5] = { 0 };
        char szFolder[MAX_PATH] = { 0 };
        char szFile[MAX_PATH] = { 0 };
        _splitpath_s( pMp3, szDrive, sizeof(szDrive)/sizeof(char), szFolder, sizeof(szFolder)/sizeof(char), szFile, sizeof(szFile)/sizeof(char), NULL, 0 );
        strcpy_s( szDstDir, MAX_PATH, szDrive );
        strcat_s( szDstDir, MAX_PATH, szFolder );
        strcat_s( szDstDir, MAX_PATH, file.GetArtist() );

        if ( !PathFileExistsA( szDstDir ) ) {
            if ( !CreateDirectoryA( szDstDir, NULL ) ) {
                std::cout << "Create File " << szDstDir << " Failed!" << endl;
                return 1;
            }
        }

        char szDst[MAX_PATH] = { 0 };
        strcpy_s( szDst, MAX_PATH, szDstDir );
        strcat_s( szDst, MAX_PATH, "\\" );
        strcat_s( szDst, MAX_PATH, szFile );
        strcat_s( szDst, MAX_PATH, ".mp3" );

        switch ( type ) {
            case EmProcMp3FileType_Copy:
                {
                    if ( CopyFileA( pMp3, szDst, FALSE ) ) {
                        cout << "Copy File " << pMp3 << " To " << szDst << " Succeed!" << endl;
                        return 0;
                    }
                }
                break;
            case EmProcMp3FileType_Move:
                {
                    if ( MoveFileA( pMp3, szDst ) ) {
                        cout << "Move File " << pMp3 << " To " << szDst << " Succeed!" << endl;
                        return 0;
                    }
                }
                break;
            default:
                break;
        }
    }

    return 1;
}

int HelpInfo() {
    cout << "mp3filecat -t c|m \"directory\"" << endl;
    cout << "The argument \"-t c \" means copy file to category." << endl;
    cout << "The argument \"-t m \" means move file to category." << endl;

    return 0;
}

int main( int argc, char** argv )
{
    //cout << argc << endl;

    EmProcMp3FileType emftType = EmProcMp3FileType_Copy;

    USES_CONVERSION;

    if ( 1 >= argc ) {
        return HelpInfo();
    }
    else {
        if ( 0 == strcmp( argv[1], "-t" ) ||
            0 == strcmp( argv[1], "-T" ) ) {
            if ( argc > 3 ) {
                if ( 0 == strcmp( argv[2], "C" ) ||
                    0 == strcmp( argv[2], "c" ) ) {
                    ;
                }
                else if ( 0 == strcmp( argv[2], "M" ) ||
                    0 == strcmp( argv[2], "m" ) ) {
                    emftType = EmProcMp3FileType_Move;
                }
                else {
                    return HelpInfo();
                }
            }
            else {
                return HelpInfo();
            }
        }
    }

    CFileFind finder;
    CString strPath( A2T( argv[3] ) );
    BOOL bWorking = finder.FindFile( strPath + TEXT( "\\*.mp3" ) );
    while ( bWorking ) {
        bWorking = finder.FindNextFile();
        if ( finder.IsDots() || finder.IsReadOnly() )
            continue;
        else if ( finder.IsDirectory() ) {
            ;
        }
        else {
            ProcMp3File( T2A( finder.GetFilePath() ), emftType );
        }
    }

    finder.Close();

    return 0;
}