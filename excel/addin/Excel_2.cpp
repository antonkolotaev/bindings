// Excel_2.cpp�: impl�mentation des exportations de DLL.


#include "stdafx.h"
#include "resource.h"

// L'attribut du module a provoqu� l'impl�mentation automatique de DllMain, DllRegisterServer et DllUnregisterServer
[ module(dll, uuid = "{442161C1-B454-4A7D-86CC-210E8FE9AAAF}", 
		 name = "Excel_2", 
		 helpstring = "Biblioth�que de types Excel_2 1.0",
		 resource_name = "IDR_EXCEL_2") ]
class CExcel_2Module
{
public:
    BOOL WINAPI DllMain(DWORD dwReason, LPVOID lpReserved) {
        return __super::DllMain(dwReason, lpReserved);
    }
};
		 
