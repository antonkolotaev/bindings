========================================================================
    ACTIVE TEMPLATE LIBRARY : Vue d'ensemble du projet Excel_2
========================================================================

AppWizard a cr�� ce projet Excel_2 pour vous permettre de l'utiliser comme point de d�part pour
l'�criture de votre biblioth�que de liens dynamiques (DLL).

Ce projet est impl�ment� avec des attributs Visual C++.

Ce fichier contient un r�sum� des �l�ments contenus dans chaque fichier qui
constitue votre projet.

Excel_2.vcproj
    Il s'agit du fichier projet principal pour les projets VC++ g�n�r�s � l'aide d'un Assistant Application. 
    Il contient des informations relatives � la version de Visual C++ qui a g�n�r� le fichier, 
    ainsi que des informations sur les plates-formes, configurations et fonctionnalit�s du projet
    d'un Assistant Application.

_Excel_2.idl
    Ce fichier sera cr�� par le compilateur lors de la g�n�ration du projet. Il contiendra les d�finitions IDL 
    de la biblioth�que de types, des interfaces et des co-classes d�finies dans votre projet.
    Ce fichier sera trait� par le compilateur MIDL pour g�n�rer :
        des d�finitions d'interface C++ et des d�clarations GUID (_Excel_2.h)
        des d�finitions GUID                                (_Excel_2_i.c)
        une biblioth�que de types                                  (_Excel_2.tlb)
        du code de marshaling                                 (_Excel_2_p.c et dlldata.c)

Excel_2.cpp
    Ce fichier contient la table des objets et l'impl�mentation des exportations de votre DLL.

Excel_2.rc
    Il s'agit de la liste de toutes les ressources Microsoft Windows que le
    programme utilise.

Excel_2.def
    Ce fichier de d�finition de module fournit � l'�diteur de liens des informations sur les exportations
    requises par votre DLL. Il contient des exportations des �l�ments suivants :
        DllGetClassObject  
        DllCanUnloadNow
        GetProxyDllInfo    
        DllRegisterServer	
        DllUnregisterServer

/////////////////////////////////////////////////////////////////////////////
Autres fichiers standard :

StdAfx.h, StdAfx.cpp
    Ces fichiers sont utilis�s pour g�n�rer un fichier d'en-t�te pr�compil� (PCH)
    nomm� Excel_2.pch et un fichier de types pr�compil�s nomm� StdAfx.obj.

Resource.h
    Il s'agit du ficher d'en-t�te standard, qui d�finit les ID de ressources.

/////////////////////////////////////////////////////////////////////////////
Projet DLL proxy/stub et fichier de d�finition de module :

Excel_2ps.vcproj
    Il s'agit du fichier projet pour la g�n�ration d'une DLL proxy/stub le cas �ch�ant.
	Le fichier IDL contenu dans le projet principal doit contenir au moins une interface 
	et vous devez d'abord compiler le fichier IDL avant de g�n�rer la DLL proxy/stub.	Ce processus g�n�re
	dlldata.c, Excel_2_i.c et Excel_2_p.c qui sont n�cessaires
	� la g�n�ration de la DLL proxy/stub.

Excel_2ps.def
    Ce fichier de d�finition de module fournit � l'�diteur de liens des informations sur les exportations
    requises par le proxy/stub.

/////////////////////////////////////////////////////////////////////////////
