/* C++ dclnam.dcl - Miscellaneous name declaration routines */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  01.18.91  PKT  009  Added GET_MFPs for member function pointer type.
 *  11.26.90  TLF  008  Added GET_ARRAY_APPLY_NAME.
 *  11.12.90  TLF  007  Added GET_XROOT_VARIABLE_NAME.
 *  10.25.90  TLF  006  Added GET_XLINK_CLEANUP_MEMBER_NAME,
 *                            GET_XLINK_NEXT_MEMBER_NAME,
 *                            GET_XLINK_STARTUP_MEMBER_NAME,
 *                            GET_XLINK_STARTUP_PROC_NAME,
 *                            GET_XLINK_TYPE_NAME,
 *                            GET_XLINK_STRUCT_NAME.
 *  10.18.90  TLF  005  Added GET_COPY_PARAM_NAME and
 *                      GET_ASSIGN_PARAM_NAME.
 *                      DECLARE_ASSIGN_OPERATOR.    
 *  10.10.90  DGM  004  Added GET_PURE_VIRTUAL_FUNCTION_NAME.
 *  09.14.90  DGM  003  Changed include file names and VOP names.
 *  09.05.90  PKT  002  Added GET_ARRAY_INIT_NAME, GET_ARRAY_FINI_NAME, 
 *			GET_DTOR_RETURN_LABEL_NAME.
 *  09.04.90  DGM  001  Added GET_MAIN_NAME.
 *  07.13.90  DGM  000  Original.
 *
 ***********************************************************************/

declare

GET_MAIN_NAME			entry
				returns  (type (NID_T))
				external (X_GET_MAIN_NAME),
GET_THIS_PARAMETER_NAME		entry
				returns  (type (NID_T))
				external (X_GET_THIS_PARAMETER_NAME),
GET_RETURN_PARAMETER_NAME	entry
				returns  (type (NID_T))
				external (X_GET_RETURN_PARAMETER_NAME),
GET_VFT_NAME			entry
				returns  (type (NID_T))
				external (X_GET_VFT_NAME),
GET_VFT_POINTER_NAME		entry
				returns  (type (NID_T))
				external (X_GET_VFT_POINTER_NAME),
GET_VFT_ENTRY_TYPE_NAME		entry
				returns  (type (NID_T))
				external (X_GET_VFT_ENTRY_TYPE_NAME),
GET_VFT_TYPE_NAME		entry
				returns  (type (NID_T))
				external (X_GET_VFT_TYPE_NAME),
GET_VFT_POINTER_TYPE_NAME	entry
				returns  (type (NID_T))
				external (X_GET_VFT_POINTER_TYPE_NAME),
GET_VFT_FUNCTION_ADDRESS_NAME	entry
				returns  (type (NID_T))
				external (X_GET_VFT_FUNCTION_ADDRESS_NAME),
GET_VFT_DELTA_NAME		entry
				returns  (type (NID_T))
				external (X_GET_VFT_DELTA_NAME),
GET_NOT_MOST_DERIVED_FLAG_NAME	entry
				returns  (type (NID_T))
				external (X_GET_NOT_MOST_DERIVED_FLAG_NAME),
GET_ARRAY_INIT_NAME		entry
				returns  (type (NID_T))
				external (X_GET_ARRAY_INIT_NAME),
GET_ARRAY_FINI_NAME		entry
				returns  (type (NID_T))
				external (X_GET_ARRAY_FINI_NAME),
GET_DTOR_RETURN_LABEL_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_DTOR_RETURN_LABEL_NAME),

GET_PURE_VIRTUAL_FUNCTION_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_PURE_VIRTUAL_FUNCTION_NAME),

GET_COPY_PARAM_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_COPY_PARAM_NAME),

GET_ASSIGN_PARAM_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_ASSIGN_PARAM_NAME),

GET_XLINK_CLEANUP_MEMBER_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_CLEANUP_MEMBER_NAME),

GET_XLINK_NEXT_MEMBER_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_NEXT_MEMBER_NAME),

GET_XLINK_STARTUP_MEMBER_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_STARTUP_MEMBER_NAME),

GET_XLINK_STARTUP_PROC_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_STARTUP_PROC_NAME),

GET_XLINK_TYPE_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_TYPE_NAME),

GET_XLINK_CLEANUP_PROC_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_CLEANUP_PROC_NAME),

GET_XLINK_STRUCT_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_XLINK_STRUCT_NAME),

GET_XROOT_VARIABLE_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_XROOT_VARIABLE_NAME),

GET_ARRAY_APPLY_NAME		entry
				returns  (type (NID_T))
				external (X_GET_ARRAY_APPLY_NAME),

GET_MFP_TYPE_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_MFP_TYPE_NAME),

GET_MFP_THIS_DELTA_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_MFP_THIS_DELTA_NAME),

GET_MFP_VIRTUAL_INDEX_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_MFP_VIRTUAL_INDEX_NAME),

GET_MFP_ADDRESS_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_MFP_ADDRESS_NAME),

GET_MFP_VIRTUAL_DELTA_NAME	entry
				returns	 (type (NID_T))
				external (X_GET_MFP_VIRTUAL_DELTA_NAME),

GET_MFP_UNION_NAME		entry
				returns	 (type (NID_T))
				external (X_GET_MFP_UNION_NAME);
