//
// C++ Header %full_filespec: exp_ilr_interface_user.hxx~5:h++:EXP#1 %
//
//  ABSTRACT:
//
//
#ifndef _EXP_1_exp_ilr_interface_user_hxx_HXX
#define _EXP_1_exp_ilr_interface_user_hxx_HXX

#pragma ident "@(#)%full_filespec: exp_ilr_interface_user.hxx~5:h++:EXP#1 %"


/*!
  This 'stub' class allows to customize the interactions with the Ilog Inference
  Engine.
 */
class EXPIlrInterfaceUser : public EXPIlrInterfaceGen
{
public:

  // Default constructor : 
  EXPIlrInterfaceUser();
  
  // Destructor
  virtual ~EXPIlrInterfaceUser();

  // Specialised methods for Rules

  /*! Declare the new Context defined by the user*/
  const char* OnCreateIlrContext ();

  /*! Import Rules user packages*/
  void OnImportUserPackages();

  // To controll how the rules are fired : specialize the following
  // functions if you want to specialize the way rules are fired.

  // int OnFireAllRules();

  // Note that advanced customization may lead to overload other methods defined
  // as Onxxx() in exp_ilr_interface.hxx file

};


#endif // _EXP_1_exp_ilr_interface_user_hxx_HXX

