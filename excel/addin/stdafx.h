// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s

#pragma once

#ifndef STRICT
#define STRICT
#endif

// Modifiez les d�finitions suivantes si vous devez cibler une plate-forme avant celles sp�cifi�es ci-dessous.
// Reportez-vous � MSDN pour obtenir les derni�res informations sur les valeurs correspondantes pour les diff�rentes plates-formes.
#ifndef WINVER				// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.
#define WINVER 0x0501		// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�98 ou version ult�rieure.
#define _WIN32_WINDOWS 0x0410 // Attribuez la valeur appropri�e � cet �l�ment pour cibler Windows�Me ou version ult�rieure.
#endif

#ifndef _WIN32_IE			// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Internet Explorer�6.0 ou version ult�rieure.
#define _WIN32_IE 0x0600	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions d'Internet Explorer.
#endif

#pragma warning ( disable : 4312 4244 )
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>


#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// certains constructeurs CString seront explicites

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlwin.h>

#include <WTL/atlapp.h>
#include <WTL/atlgdi.h>
#include <WTL/atlddx.h>
#include <WTL/atlmisc.h>
#include <WTL/atlctrls.h>
extern CAppModule _Module;
#undef REGENERATE_TYPELIBRARIES
#ifndef REGENERATE_TYPELIBRARIES 

using namespace ATL;
#include "generated_tlbs/msaddndr.tlh"
#include "generated_tlbs/mso.tlh"
using namespace Office;

#include "generated_tlbs/vbe6ext.tlh"
using namespace VBIDE;

#include "generated_tlbs/excel.tlh"
#include "generated_tlbs/fm20.tlh"

#else


using namespace ATL;
#import "C:\Program Files\Fichiers communs\Designer\MSADDNDR.DLL" raw_interfaces_only, raw_native_types, no_namespace, named_guids, auto_search

#import "C:\\Program Files\\Fichiers communs\\Microsoft Shared\\OFFICE11\\MSO.DLL" \
    rename( "RGB", "MSORGB" )\
    rename( "DocumentProperties", "_DocumentProperties" )

using namespace Office;

#import "C:\\Program Files\\Fichiers communs\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.OLB" 

using namespace VBIDE;

#import "C:\\Program Files\\Microsoft Office\\OFFICE11\\EXCEL.EXE" \
    rename( "DialogBox", "ExcelDialogBox" ) \
    rename( "RGB", "ExcelRGB" ) \
    rename( "CopyFile", "ExcelCopyFile" ) \
    rename( "ReplaceText", "ExcelReplaceText" )\
    exclude("IFont", "IPicture")\
    rename_namespace("Excel")

#import "C:\\Windows\\System32\\fm20.dll"
//#import "C:\DOCUME~1\kolotaev\LOCALS~1\Temp\Excel8.0\MSForms.exd" raw_interfaces_only, raw_native_types, no_namespace, named_guids, auto_search

#endif

/// Converts an integer to a string.
/// To be replaced to boost::lexical_cast
inline _bstr_t toString(int i)
{
    char buffer[20];
    _itoa_s(i, buffer, 10);
    return buffer;
}