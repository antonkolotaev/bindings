// OptionPricer.h�: d�claration de COptionPricer

#pragma once
#include "resource.h"       // symboles principaux



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Les objets COM monothread ne sont pas correctement pris en charge par les plates-formes Windows CE, notamment les plates-formes Windows Mobile qui ne prennent pas totalement en charge DCOM. D�finissez _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA pour forcer ATL � prendre en charge la cr�ation d'objets COM monothread et permettre l'utilisation de leurs impl�mentations. Le mod�le de thread de votre fichier rgs a �t� d�fini sur 'Libre' car il s'agit du seul mod�le de thread pris en charge par les plates-formes Windows CE non-DCOM."
#endif

namespace prxl {

/// Interface for running a pricing method 
[
	object,
	uuid("5A8C0ABE-B12A-4F07-AE9E-16EA3FD3F445"),
	dual,	helpstring("Interface IOptionPricer"),
	pointer_default(unique)
]
__interface IOptionPricer : IDispatch
{
    /// Initializes parameters to run a pricing method
    /// \param requested_param -- a string with name of a field requested, e.g. "Price"
    /// \param model_id - a string with a model instance label, e.g. "BlackScholes1dim:0"
    /// \param option_id - a string with an option instance label, e.g. "CallEuro:0"
    /// \param method_id - a string with a priocing method label, e.g. "CF_Call:1"
    [id(2), helpstring("m�thode Init")] HRESULT Init([in] BSTR requested_param, [in] BSTR model_id, [in] BSTR option_id, [in] BSTR method_id);
    
    /// Returns iteration extents.
    /// \param pSize_1 - output parameter used to save number of element in the first iteration
    /// \param pSize_2 - output parameter used to save number of element in the second iteration
    [id(3), helpstring("m�thode GetExtents")] HRESULT GetExtents([out] long* pSize_1, [out] long* pSize_2);

    /// Launches the pricing method
    /// \param it_1 - index of the first iteration if present
    /// \param it_2 - index of the second iteration if present
    /// \pRes - output parameter used to save a result of the computation
    [id(4), helpstring("m�thode Run")] HRESULT Run([in] long it_1, [in] long it_2, [out,retval] double* pRes);
};



/// Implementation of IOptionPricer
[
	coclass,
	default(IOptionPricer),
	threading(apartment),
	vi_progid("Excel_2.OptionPricer"),
	progid("Excel_2.OptionPricer.1"),
	version(1.0),
	uuid("70D3309E-6E09-4625-895F-EBFAD80AF875"),
	helpstring("OptionPricer Class")
]
class ATL_NO_VTABLE COptionPricer :
	public IOptionPricer
{
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

    /// Initializes parameters to run a pricing method
    /// \param requested_param -- a string with name of a field requested, e.g. "Price"
    /// \param model_id - a string with a model instance label, e.g. "BlackScholes1dim:0"
    /// \param option_id - a string with an option instance label, e.g. "CallEuro:0"
    /// \param method_id - a string with a priocing method label, e.g. "CF_Call:1"
    HRESULT __stdcall Init(BSTR requested_param, BSTR model_id, BSTR option_id, BSTR method_id);

    /// Returns iteration extents.
    /// \param pSize_1 - output parameter used to save number of element in the first iteration
    /// \param pSize_2 - output parameter used to save number of element in the second iteration
    HRESULT __stdcall GetExtents(long* pSize_1, long* pSize_2);

    /// Launches the pricing method
    /// \param it_1 - index of the first iteration if present
    /// \param it_2 - index of the second iteration if present
    /// \pRes - output parameter used to save a result of the computation
    HRESULT __stdcall Run(long it_1, long it_2, double* pRes);

private:

    struct Impl;
    /// a pointer to the pImpl
    std::auto_ptr<Impl> impl_;
};

}