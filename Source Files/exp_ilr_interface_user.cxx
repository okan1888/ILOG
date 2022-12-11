// @(#) C++ Source %full_filespec: exp_ilr_interface_user.cxx~6:c++:EXP#1 %
//
//
//

#pragma ident "@(#) %full_filespec: exp_ilr_interface_user.cxx~6:c++:EXP#1 %"


#include "exp_generated_api.hxx"

#include "exp_ilr_interface_user.hxx" 
#include "temip_entity_user.h"

// tracing facility
#include "exp_trace_defs.hxx"
#define MM_K_TRACE_COMPONENT_ID EXP_USERCUST_COMPONENT_TRACE_ID
#define MM_K_TRACE_COMPONENT_NAME EXP_USERCUST_COMPONENT_TRACE_NAME
#include "mm_trace.hxx"

// *************
// constructor 
// ************* 

EXPIlrInterfaceUser::EXPIlrInterfaceUser()
	:EXPIlrInterfaceGen()
{
}

// *************
// destructor 
// ************* 
  
EXPIlrInterfaceUser::~EXPIlrInterfaceUser()
{
}

// Specialised methods for Rules
const char* EXPIlrInterfaceUser::OnCreateIlrContext()
{
  //  Return the full qualified name of the context class (as declared in the .ilr file)
  return "user_model.TemipExpertContextUser";
}

void EXPIlrInterfaceUser::OnImportUserPackages()
{
  IMPORT_USER_PACKAGE(temip_entity_userPackage); 
 IMPORT_USER_PACKAGE(user_modelPackage);
  // the name "user_modelPackage" is constructed from the name of the
  // package (defined in user_model.ilr) concatenated with the keyword "Package"
}


//***************************
// Control on ILog Rules Use
//***************************

// To controll how the rules are fired : specialize the following
// functions if you want to specialize the way rules are fired.

// int EXPIlrInterfaceUser::OnFireAllRules()
// {
//   return GetIlrContext().EXPIlrInterface::OnFireAllRules();
// } 

