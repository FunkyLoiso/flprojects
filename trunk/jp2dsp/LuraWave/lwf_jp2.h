/*--------------------------------------------------------------------------*/
/*																			*/
/*	LuraTech JPEG-2000 Project												*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*																			*/
/*	Company:	LuraTech Imaging GmbH										*/
/*	Author:		LuraTech JPEG 2000 Group									*/
/*	Version:	2.1.00.08 (May 2008)										*/
/*																			*/
/*--------------------------------------------------------------------------*/

#ifndef _LWF_JP2_H_
#define _LWF_JP2_H_

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef JP2_DLL_EXPORTS
#define		JP2_API		__declspec(dllexport)
#else
#define		JP2_API
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Version Information														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		LWF_JP2_VERSION             2.08
#define		LWF_JP2_VERSION_STRING      "v2.1.00.08 (May 2008)"
#define		LWF_JP2_LICENSE_LEVEL		0

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Error codes																*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Error_OK									 0
#define		cJP2_Error_Failure_Malloc						-1
#define		cJP2_Error_Failure_Read							-2
#define		cJP2_Error_Failure_Write						-3
#define		cJP2_Error_Invalid_Handle						-4
#define		cJP2_Error_Invalid_Pointer						-5
#define		cJP2_Error_Invalid_Number_Of_Components			-6
#define		cJP2_Error_Invalid_Component_Index				-7
#define		cJP2_Error_Invalid_Property_Value				-8
#define		cJP2_Error_Invalid_Property_Key					-9
#define		cJP2_Error_Invalid_Width						-10
#define		cJP2_Error_Invalid_Height						-11
#define		cJP2_Error_Invalid_Bits_Per_Sample				-12
#define		cJP2_Error_Invalid_Tile_Arrangement				-13
#define		cJP2_Error_Invalid_Colorspace					-14
#define		cJP2_Error_Invalid_Component_Dimensions			-15
#define		cJP2_Error_Invalid_Tile_Index					-16
#define		cJP2_Error_Invalid_Resolution_Level				-17
#define		cJP2_Error_Invalid_Wavelet_Filter_Combination	-18
#define		cJP2_Error_Invalid_Stream						-19
#define		cJP2_Error_Single_Value_For_All_Components		-20
#define		cJP2_Error_Single_Value_For_All_Tiles			-21
#define		cJP2_Error_Read_Only_Property					-22
#define		cJP2_Error_Set_Only_Property					-23
#define		cJP2_Error_Quality_Compression_Mode				-24
#define		cJP2_Error_Decompression_Only_Property			-25
#define		cJP2_Error_Compression_Only_Property			-26
#define		cJP2_Error_Bits_Per_Sample_Too_High				-27
#define		cJP2_Error_Input_Callback_Undefined				-28
#define		cJP2_Error_Write_Callback_Undefined				-29
#define		cJP2_Error_Read_Callback_Undefined				-30
#define		cJP2_Error_Cannot_Find_Suitable_Grid			-31
#define		cJP2_Error_Byte_Compression_Mode				-32
#define		cJP2_Error_Requested_File_Size_Too_Small		-33
#define		cJP2_Error_Invalid_Sample_Rate					-34
#define		cJP2_Error_Not_Yet_Supported					-35
#define		cJP2_Error_Trial_Time_Expired					-36
#define		cJP2_Error_Invalid_Quantization_Filter_Pair		-37
#define		cJP2_Error_Invalid_Precinct_Dimensions			-38
#define		cJP2_Error_ROI_Shift_Failed						-39
#define		cJP2_Error_Scale_Factor_Is_Too_Large			-40
#define		cJP2_Error_Invalid_Resolution					-41
#define		cJP2_Error_Invalid_Resolution_Unit				-42
#define		cJP2_Error_Invalid_Resolution_Type				-43
#define		cJP2_Error_Max_Number_Of_ROIs_Reached			-45
#define		cJP2_Error_More_Bytes_Required					-50
#define		cJP2_Error_Decompression_Cancelled				-51
#define		cJP2_Error_File_Format_Required					-52
#define		cJP2_Error_JPX_File_Format_Required				-53
#define		cJP2_Error_Invalid_Meta_Data_Box_Index			-54
#define		cJP2_Error_Invalid_Color_Spec_Index				-55
#define		cJP2_Error_Invalid_ICC_Profile					-56
#define		cJP2_Error_Use_SetICC_Function					-57
#define		cJP2_Error_Use_SetLAB_Function					-58
#define		cJP2_Error_Missing_Palette						-59
#define		cJP2_Error_Invalid_Palette						-60
#define		cJP2_Error_Missing_Component_Mapping			-61
#define		cJP2_Error_Invalid_Component_Mapping			-62
#define		cJP2_Error_Invalid_Channel_Definition			-63
#define		cJP2_Error_Use_SetPalette						-64
#define		cJP2_Error_Transcoding_Finished					-65
#define		cJP2_Error_Transcode_Scale_Palette_Images		-66
#define		cJP2_Error_Invalid_Region						-67
#define		cJP2_Error_Lossless_Compression_Mode			-68
#define		cJP2_Error_Maximum_Box_Size_Exceeded			-69
#define		cJP2_Error_Invalid_Label               			-70
#define     cJP2_Error_Invalid_Header                       -71
#define     cJP2_Error_Incompatible_Format                  -72
#define		cJP2_Error_Invalid_Marker						-73

#define		cJP2_Error_Invalid_License						-90
#define		cJP2_Error_License_Level_Too_Low				-91

#define		cJP2_Error_Fatal								-100

#define		cJP2_Warning_Unable_To_Read_All_Data			 10

typedef long	JP2_Error;


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Property values															*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifdef WIN64
typedef unsigned long long			JP2_Property_Value;
typedef          long long			JP2_Callback_Param;
#else
typedef unsigned long				JP2_Property_Value;
typedef	         long				JP2_Callback_Param;
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Callback function prototypes											*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	Calling convention for callbacks.										*/


#ifdef	WIN32
#define		JP2_Callback_Conv		__cdecl
#else
#define		JP2_Callback_Conv
#endif

typedef void *			(JP2_Callback_Conv *JP2_Malloc_Func)(
										long					lSize,
										JP2_Callback_Param		lMallocParam);

typedef JP2_Error		(JP2_Callback_Conv	*JP2_Free_Func)	(
										void*					pMemory,
										JP2_Callback_Param		lFreeParam);

typedef JP2_Error		(JP2_Callback_Conv *JP2_Input_Func) (
										unsigned char*			pucData, 
										short					sComponent, 
										unsigned long			ulRow, 
										unsigned long			ulStart,
										unsigned long			ulNum,
										JP2_Callback_Param		lInputParam);

typedef JP2_Error		(JP2_Callback_Conv *JP2_Write_Func) (
										unsigned char*			pucData, 
										unsigned long			ulPos, 
										unsigned long			ulSize,
										JP2_Callback_Param		lWriteParam);

typedef unsigned long	(JP2_Callback_Conv *JP2_Read_Func)	(
										unsigned char*			pucData, 
										unsigned long			ulPos, 
										unsigned long			ulSize,
										JP2_Callback_Param		lReadParam);

typedef JP2_Error		(JP2_Callback_Conv *JP2_Output_Func)(
										unsigned char*			pucData,
										short					sComponent, 
										unsigned long			ulRow, 
										unsigned long			ulStart,
										unsigned long			ulNum,
										JP2_Callback_Param		lOutputParam);

typedef long			(JP2_Callback_Conv	*JP2_Cancel_Func)(
										JP2_Callback_Param		lCancelParam);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Properties																*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/****************************************************************************/
/*	General image properties												*/
/****************************************************************************/

/*	The following general image properties are read-only during decoding	*/
/*	and transcoding															*/
/*																			*/
/*	The cJP2_Prop_Components parameter may only may be set as parameter		*/
/*	when calling JP2_Compress_Start.  It may not be set using				*/
/*	JP2_Compress_SetProp.													*/
/*																			*/
/*	The cJP2_Prop_Num_Tiles parameters is read-only.  The number of tiles	*/
/*	depends on the the tile	dimensions.  The value of cJP2_Prop_Num_Tiles	*/
/*	is automatically calculated.											*/
/*																			*/
/*	The properties cJP2_Prop_Image_X, cJP2_Prop_Image_Y,					*/
/*	cJP2_Prop_Tile_Width, cJP2_Prop_Tile_Height, cJP2_Prop_Tile_X,			*/
/*	cJP2_Prop_Tile_Y are set during coding with respect to the current grid	*/
/*	grid.  If the grid needs to be recalculated then the values of these	*/
/*	properties will be appropriately scaled.								*/
/*																			*/
/*	Properties cJP2_Prop_Grid_Width and cJP2_Grid_Height are read-only		*/
/*	and are automatically calculated with respect to the current values of	*/
/*	cJP2_Prop_Width and cJP2_Prop_Height for all components.				*/
/*																			*/
/*	cJP2_Prop_Width				: width of an image component				*/
/*								: range is 1 - (2^32 - 1)					*/
/*								: no default								*/
/*	cJP2_Prop_Height			: height of an image component				*/
/*								: range is 1 - (2^32 - 1)					*/
/*								: no default								*/
/*	cJP2_Prop_Components		: number of components (channels)			*/
/*								: range is 1 - 16384						*/
/*								: no default								*/
/*	cJP2_Prop_Bits_Per_Sample	: precision of component samples			*/
/*								: range is 1 - 28							*/
/*								: no default								*/
/*	cJP2_Prop_Signed_Samples	: are component samples signed?				*/
/*								: range is 0 (unsigned) - 1 (signed)		*/
/*								: default is 0 (unsigned)					*/
/*	cJP2_Prop_Tile_Width		: width of one tile in reference grid		*/
/*								: range is 0 - (2^32 - 1)					*/
/*								: a value of 0 indicates grid width			*/
/*								: currently only tile widths which are a	*/
/*								  power of 2 and at least 1024 are supported*/
/*								: default is grid width (0)					*/
/*	cJP2_Prop_Tile_Height		: height of one tile in reference grid		*/
/*								: range is 0 - (2^32 - 1)					*/
/*								: a value of 0 indicates grid height		*/
/*								: currently only tile heights which are a	*/
/*								  power of 2 and at least 1024 are supported*/
/*								: default is grid height (0)				*/
/*	cJP2_Prop_Num_Tiles			: number of tiles							*/
/*								: read-only property						*/
/*								: calculated using grid and tile position	*/
/*								  and dimensions							*/
/*	cJP2_Prop_File_Format		: file format								*/
/*								: cJP2_Format_Codestream, cJP2_Format_JP2	*/
/*								: or cJP2_Format_JPX                        */
/*								: default is cJP2_Format_JP2				*/
/*	cJP2_Prop_Grid_Width		: width of reference grid					*/
/*								: read-only property						*/
/*								: range is 1 - (2^32 - 1)					*/
/*								: automatically calculated during coding	*/  
/*	cJP2_Prop_Grid_Height		: height of reference grid					*/
/*								: read-only property						*/
/*								: range is 1 - (2^32 - 1)					*/
/*								: automatically calculated during coding	*/  
/*	cJP2_Prop_Image_X			: image horizontal offset from grid origin	*/
/*								: range is 0 - (2^32 - 2)					*/
/*								: default is 0								*/
/*	cJP2_Prop_Image_Y			: image vertical offset from grid origin	*/
/*								: range is 0 - (2^32 - 2)					*/
/*								: default is 0								*/
/*	cJP2_Prop_Tile_X			: tile horizontal offset from grid origin	*/
/*								: range is 0 - (2^32 - 2)					*/
/*								: default is 0								*/
/*	cJP2_Prop_Tile_Y			: tile vertical offset from grid origin		*/
/*								: range is 0 - (2^32 - 2)					*/
/*								: default is 0								*/

#define		cJP2_Prop_Width									   1
#define		cJP2_Prop_Height								   2
#define		cJP2_Prop_Components							   3
#define		cJP2_Prop_Bits_Per_Sample						   4
#define		cJP2_Prop_Signed_Samples						   5
#define		cJP2_Prop_Tile_Width							   6
#define		cJP2_Prop_Tile_Height							   7
#define		cJP2_Prop_Num_Tiles								   8
#define		cJP2_Prop_File_Format							   9
#define		cJP2_Prop_Grid_Width							  10
#define		cJP2_Prop_Grid_Height							  11
#define		cJP2_Prop_Image_X								  12
#define		cJP2_Prop_Image_Y								  13
#define		cJP2_Prop_Tile_X								  14
#define		cJP2_Prop_Tile_Y								  15

/*	The following general image property may be set during decoding and		*/
/*	encoding.																*/
/*																			*/
/*	cJP2_Prop_Precise			: force use of 32 bit of values during		*/
/*								  wavelet and colour transformations.		*/
/*								: default is 0 -> only use 32 bits when		*/
/*								  necessary.								*/
/*								: range is 0 - 1.							*/

#define		cJP2_Prop_Precise								  16

/****************************************************************************/
/*	Transcoding properties													*/
/****************************************************************************/

/*	The following properties may be set when transcoding a compressed image	*/
/*																			*/
/*	- cJP2_Prop_Scale_Down													*/
/*	- cJP2_Prop_Scale_Up													*/
/*	- cJP2_Prop_Progression													*/
/*	- cJP2_Prop_Write_Parameter												*/
/*																			*/
/*	- cJP2_Prop_Write_Function MUST be set									*/
/*																			*/
/*	See below for full details of each of these properties.					*/

/****************************************************************************/
/*	General input and output properties										*/
/****************************************************************************/

/*	These properties are the only properties which can be set during		*/
/*	decoding																*/
/*																			*/
/*	cJP2_Prop_Extern_Colorspace	: colorspace used for input of image data	*/
/*								: or colorspace required for output of data	*/
/*								: see defined JP2_Colorspace values			*/
/*								: default is cJP2_Colorspace_Gray			*/
/*	cJP2_Prop_Byte_Order		: byte order for input/output of image data	*/
/*								: cJP2_Endian_Little or cJP2_Endian_Big		*/
/*								: default is machine native order			*/
/*	cJP2_Prop_Scale_Down		: scaling down factor for image				*/
/*								: value must be a power of 2				*/
/*								: default is 1								*/
/*	cJP2_Prop_Scale_Up			: scaling up factor for image				*/
/*								: value must be a power of 2				*/
/*								: default is 1								*/
/*	cJP2_Prop_InternalReadCache	: set use of internal cache					*/
/*								: cJP2_UseInternalCache or					*/
/*								  cJP2_NoInternalCache						*/
/*								: default is cJP2_UseInternalCache			*/
/*	cJP2_Prop_Expand_Bitonal	: whether bitonal images should be expanded	*/
/*								  to 8-bit grayscale during decoding.		*/
/*								: 0 (no) - 1 (yes).							*/
/*								: default is 0 (no).						*/
/*								: Decoding bitonal images as grayscale can	*/
/*								  improve image quality when downscaling is	*/
/*								  used.										*/
/*																			*/
/*	Note on scaling:														*/
/*																			*/
/*	Setting either cJP2_Prop_Scale_Up or cJP2_Prop_Scale_Down will over		*/
/*	write any previous settings using these properties.						*/
/*	The width and height of the scaled image can be obtained after setting	*/
/*	the scaling factor by calling JP2_Decompress_GetProp with properties	*/
/*	cJP2_Prop_Width and cJP2_Prop_Height.									*/
/*																			*/

#define		cJP2_Prop_Extern_Colorspace						  20	
#define		cJP2_Prop_Byte_Order							  21
#define		cJP2_Prop_Scale_Down							  23
#define		cJP2_Prop_Scale_Up								  24
#define		cJP2_Prop_InternalReadCache						  25
#define		cJP2_Prop_Expand_Bitonal						  26

/****************************************************************************/
/*	Compression callback properties											*/
/****************************************************************************/

/*	cJP2_Prop_Input_Function	: input uncompressed image data function	*/
/*								: JP2_Input_Func cast to long				*/
/*								: no default								*/
/*	cJP2_Prop_Input_Parameter	: input uncompressed image data parameter	*/
/*								: arbitrary structure cast to long			*/
/*								: default is 0								*/
/*	cJP2_Prop_Write_Function	: write compressed data function			*/
/*								: JP2_Write_Func cast to long				*/
/*								: no default								*/
/*	cJP2_Prop_Write_Parameter	: write parameter							*/
/*								: arbitrary structure cast to long			*/
/*								: default is 0								*/

#define		cJP2_Prop_Input_Function						  40
#define		cJP2_Prop_Input_Parameter						  41
#define		cJP2_Prop_Write_Function						  42
#define		cJP2_Prop_Write_Parameter						  43

/****************************************************************************/
/*	Decompression callback properties										*/
/****************************************************************************/

/*	cJP2_Prop_Output_Function	: output uncompressed image data function	*/
/*								: JP2_Output_Func cast to long				*/
/*								: no default								*/
/*	cJP2_Prop_Output_Parameter	: output uncompressed parameter				*/
/*								: arbitrary structure cast to long			*/
/*								: default is 0								*/
/*	cJP2_Prop_Cancel_Function	: cancel decompression query function		*/
/*								: JP2_Cancel_Func cast to long				*/
/*								: default is function returning false		*/
/*	cJP2_Prop_Cancel_Parameter	: cancel decompression query parameter		*/
/*								: arbitrary structure cast to long			*/
/*								: default is 0								*/

#define		cJP2_Prop_Output_Function						  50
#define		cJP2_Prop_Output_Parameter						  51
#define		cJP2_Prop_Cancel_Function						  52
#define		cJP2_Prop_Cancel_Parameter						  53

/****************************************************************************/
/*	Compression parameter properties										*/
/****************************************************************************/

/*	The following compression parameters may be set and apply to the		*/
/*	complete image:															*/

/*	cJP2_Prop_Rate_Bytes		: maximum number of bytes in code streams	*/
/*								: range is 1 - (2^32 - 1)					*/
/*								: default is 0 (lossless)					*/
/*	cJP2_Prop_Rate_Quality		: quality of compressed image				*/
/*								: range is 1 - 100							*/
/*								: cannot be used in combination with		*/
/*								  cJP2_Prop_Rate_Bytes, with 5-3 wavelet	*/
/*								  filter, with derived quantization or with	*/
/*								  regions of interest						*/
/*								: default is lossless compression			*/
/*	cJP2_Prop_Quality_Style		: style of visual quality optimization		*/
/*								: see defined JP2_Quality_Style values		*/
/*								: default is cJP2_Quality_PSNR				*/
/*	cJP2_Prop_Speed_Mode		: encoding speed mode						*/
/*								: see defined JP2_Speed_Mode values			*/
/*								: default is cJP2_Speed_Fast				*/
/*	cJP2_Prop_Write_TLM_Marker	: add TLM marker to codestream for better	*/
/*								  access in tiled images?					*/
/*								: range is 0 (no) - 1 (yes).				*/
/*								: default is 0 -- no TLM marker added.		*/

#define		cJP2_Prop_Rate_Bytes							  63
#define		cJP2_Prop_Rate_Quality							  64
#define		cJP2_Prop_Quality_Style							  65
#define		cJP2_Prop_Speed_Mode							  66
#define		cJP2_Prop_Write_TLM_Marker						  67

/*	The following compression parameters may be set for each tile:			*/

/*	cJP2_Prop_Packet_Markers	: error recovery markers in bit stream?		*/
/*								: range is 0 (none), 1 (SOP), 2 (EPH),		*/
/*								  3 (SOP and EPH)							*/
/*								: default is 0 (none)						*/
/*	cJP2_Prop_Progression		: progression order							*/
/*								: see defined JP2_Progression values		*/
/*								: default is cJP2_Progression_RLCP			*/
/*	cJP2_Prop_Layers			: number of layers in code stream			*/
/*								: range is 1 - 65535						*/
/*								: default is 1								*/
/*								: currently only 1 layer during coding		*/ 

#define		cJP2_Prop_Packet_Markers						  60
#define		cJP2_Prop_Progression							  61
#define		cJP2_Prop_Layers								  62

/*	The following compression parameters may be set for each component in	*/
/*	each tile.																*/
/*																			*/
/*	cJP2_Prop_Guard_Bits		: number of guard bits						*/
/*								: range is 0 - 7							*/
/*								: default is 2								*/
/*	cJP2_Prop_Wavelet_Filter	: wavelet filter							*/
/*								: see defined JP2_Wavelet_Filter values		*/
/*								: default is cJP2_Wavelet_5_3				*/
/*	cJP2_Prop_Wavelet_Levels	: number of wavelet transformation levels	*/
/*								: range is 0 - 16							*/
/*								: default is 5								*/
/*	cJP2_Prop_Codeblock_Width	: width of codeblock						*/
/*								: range, 2^xcb where 2 <= xcb <= 10	and		*/
/*								  xcb + ycb <= 12 (see Codeblock_Height)	*/
/*								: default is 64 (xcb = 6)					*/
/*	cJP2_Prop_Codeblock_Height	: height of codeblock						*/
/*								: range, 2^ycb where 2 <= ycb <= 10	and		*/
/*								  xcb + ycb <= 12 (see Codeblock_Width)		*/
/*								: default is 64 (ycb = 6)					*/
/*	cJP2_Prop_Coder_Options		: codeblock coding style options 			*/
/*								: range is 0 - 50	(see JP2_Coder_Options	*/
/*								  bit masks)								*/
/*								: default is 0								*/
/*	cJP2_Prop_Quantization_Style: quantization style						*/
/*								: range is 0 - 1 (see JP2_Quant_Styles)		*/
/*								: default is cJP2_Quant_None with 5-3 filter*/
/*								    and cJP2_Quant_Expounded with 9-7 filter*/

#define		cJP2_Prop_Guard_Bits							  70
#define     cJP2_Prop_Wavelet_Filter                          71
#define     cJP2_Prop_Wavelet_Levels                          72
#define		cJP2_Prop_Codeblock_Width						  73
#define		cJP2_Prop_Codeblock_Height						  74
#define		cJP2_Prop_Coder_Options							  75
#define		cJP2_Prop_Quantization_Style					  76

typedef long	JP2_Property_Tag;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Resolution units														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Resolution_Dots_Per_Inch					0x00
#define		cJP2_Resolution_Dots_Per_Centimeter				0x01
#define		cJP2_Resolution_Dots_Per_Meter					0x02

typedef long	JP2_Resolution_Units;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Resolution types														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Resolution_Capture							0x00
#define		cJP2_Resolution_Display							0x01

typedef long	JP2_Resolution_Types;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Resolution structure for DPI information								*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	Note that resolution information may only be stored in JP2 file format	*/
/*	images.	 Plain codestream format images can not store resolution		*/
/*	information.															*/

typedef struct
{
	/*	The image resolution is calculated by:								*/
	/*	[DPM] = ( xNum / xDenom ) * 10 ^ xExp								*/

	/* for vertical resolution (Y direction)								*/

	unsigned short	usVertNum;		/*	{    1 .. (2^16 -1) }				*/
	unsigned short	usVertDenom;	/*	{    1 .. (2^16 -1) }				*/
	char			cVertExp;		/*	{ -128 .. 127       }				*/

	/* for horizontal resolution (X direction)								*/

	unsigned short	usHorNum;		/*	{    1 .. (2^16 -1) }				*/
	unsigned short	usHorDenom;		/*	{    1 .. (2^16 -1) }				*/
	char			cHorExp;		/*	{ -128 .. 127       }				*/

}	JP2_DPM_Resolution;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	UUID structure for vendor specific information							*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
	unsigned char	pucUUID[16];	/* 16-byte UUID as specified by ISO/IEC	*/
									/* 11578:1996							*/
}	JP2_UUID;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Wavelet filters															*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Wavelet_9_7								 0
#define		cJP2_Wavelet_5_3								 1

typedef long	JP2_Wavelet_Filter;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	File formats															*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Format_Codestream							 0
#define		cJP2_Format_JP2									 1
#define		cJP2_Format_JPX									 2

typedef long	JP2_File_Format;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Coder option bit masks													*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Coder_Context_Reset						0x02
#define		cJP2_Coder_All_Pass_Terminate					0x04
#define		cJP2_Coder_Vertical_Causal_Context				0x08
#define		cJP2_Coder_Predictable							0x10
#define		cJP2_Coder_Segmentation_Symbols					0x20

typedef long	JP2_Coder_Options;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Quantization style														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	Three quantization styles are supported. cJP2_Quant_None must be used	*/
/*	with the 5-3 wavelet filter.  The 9-7 filter may be used either with	*/
/*	derived quantisation -- where the step size is only signalled for the	*/
/*  LL sub-band and the step sizes for all other bands are derived from		*/
/*	this -- or expounded quantisation -- where a step size is generated and	*/
/*	signalled for each sub-band.  The expounded quantisation style is the	*/
/*	default when using the 9-7 filter.										*/

#define		cJP2_Quant_None									0x00
#define		cJP2_Quant_Derived								0x01
#define		cJP2_Quant_Expounded							0x02

typedef long	JP2_Quant_Style;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Progression orders														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	L: layer, R: resolution level, C: component, P: position				*/

#define		cJP2_Progression_LRCP			 0
#define		cJP2_Progression_RLCP			 1
#define		cJP2_Progression_RPCL			 2
#define		cJP2_Progression_PCRL			 3
#define		cJP2_Progression_CPRL			 4

typedef long	JP2_Progression;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Encoding speed mode														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	Values for setting the encoding speed.									*/
/*	Fast mode offers higher speed but may create images with lower quality.	*/

#define		cJP2_Speed_Fast					 0
#define		cJP2_Speed_Accurate				 1

typedef long	JP2_Speed_Mode;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Quality styles															*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	Several different quality measures may be used during compression.		*/
/*	The quality may be optimised for PSNR or for visual quality. The visual	*/
/*	quality may be optimised for viewing from small, medium and large		*/
/*	distance.																*/

#define		cJP2_Quality_PSNR				 0

#define		cJP2_Quality_Visual_XXSmall		 9
#define		cJP2_Quality_Visual_XSmall		 8
#define		cJP2_Quality_Visual_Small		 1
#define		cJP2_Quality_Visual_Medium		 2
#define		cJP2_Quality_Visual_Large		 3
#define		cJP2_Quality_Visual_XLarge		 4
#define		cJP2_Quality_Visual_XXLarge		 5

typedef long	JP2_Quality_Style;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Decompression internal read cache usage									*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_UseInternalCache				0x01
#define		cJP2_NoInternalCache				0x00

typedef long	JP2_Cache_Option;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Byte Order	-	for input/output of image data							*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Endian_Little					 0
#define		cJP2_Endian_Big						 1

typedef long	JP2_Byte_Order;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	External colorspaces	-	for input/output of image data				*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Colorspace_Gray				10
#define		cJP2_Colorspace_RGBa				20
#define		cJP2_Colorspace_RGB_YCCa			30
#define		cJP2_Colorspace_ICCa				40
#define		cJP2_Colorspace_CMYKa				50
#define		cJP2_Colorspace_CIE_LABa			60

/*	Decompression only colorspaces											*/
/*	Compression of palette images is achieved by setting the normal			*/
/*	colorspace, e.g. cJP2_Colorspace_Gray, and specifying a palette using	*/
/*	the function JP2_Compress_SetPalette.									*/

#define		cJP2_Colorspace_Palette_Gray		11
#define		cJP2_Colorspace_Palette_RGBa		21
#define		cJP2_Colorspace_Palette_RGB_YCCa	31
#define		cJP2_Colorspace_Palette_ICCa		41
#define		cJP2_Colorspace_Palette_CMYKa		51
#define		cJP2_Colorspace_Palette_CIE_LABa	61

typedef long	JP2_Colorspace;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Standard Reader Requirement Features									*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	The JPX file format contains a box, the Reader Requirements box,		*/
/*	specifying the features contained within the file.  Many of these		*/
/*	features, including the colourspace and existence of IP data, are set	*/
/*	automatically by the library during	compression.						*/

/*	The following additional features are not automatically detected by the	*/
/*	library and the JP2_Compress_SetFeature function may be	called to add	*/
/*	these features to the Reader Requirement box of a JPX format file.		*/

/*	The function JP2_Decompress_ContainsFeature can be used to check		*/
/*	whether one of these features is found in a file. Details of additional	*/
/*	features, including the colourspace and existence of IP data, can be	*/
/*	obtained using functions such as JP2_Decompress_GetProp and				*/
/*	JP2_Decompress_GetIP_Num.												*/
  
#define		cJP2_Feature_ROI_Metadata			34
#define		cJP2_Feature_Content_Metadata		36
#define		cJP2_Feature_History_Metadata		37
#define		cJP2_Feature_Creation_Metadata		38
#define		cJP2_Feature_Digitally_Signed		39
#define		cJP2_Feature_Checksummed			40
#define		cJP2_Feature_GML_Metadata			67

typedef unsigned short	JP2_Standard_Feature;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Type of channels														*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define		cJP2_Channel_Type_Color				 0
#define		cJP2_Channel_Type_Opacity			 1
#define		cJP2_Channel_Type_Opacity_Pre		 2
#define		cJP2_Channel_Type_Undefined			 0xffff

typedef long	JP2_Channel_Type;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Data structure for palette images										*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
	unsigned long	ulEntries;			/*	number of palette entries		*/
	unsigned long	ulChannels;			/*	number of palette channels		*/
	unsigned char	*pucBitsPerSample;	/*	depth of palette channels		*/
	unsigned char	*pucSignedSample;	/*	sign of palette channels		*/
										/*	0 -> unsigned, 1 -> signed		*/
	unsigned long	**ppulPalette;		/*	palette component values		*/
										/*	[i][j] is value of jth entry	*/
										/*	for channel i					*/

}	JP2_Palette_Params;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Data structure for channel definitions - decompression only				*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*	Each channel in a decompressed image has a type and may be associated	*/
/*	with a particular color. See the handbook for a full explaination of	*/
/*	the channel definition structure.										*/

typedef struct
{
	JP2_Channel_Type	ulType;			/*	the type of this channel		*/
	unsigned long		ulAssociated;	/*	the color associated with this	*/
										/*	channel.						*/

}	JP2_Channel_Def_Params;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Extended parameters for the CIE L*a*b* color specification boxes		*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
	unsigned long	ulRL;					/*	Range  for L*				*/
	unsigned long	ulOL;					/*	Offset for L*				*/
	unsigned long	ulRA;					/*	Range  for a*				*/
	unsigned long	ulOA;					/*	Offset for a*				*/
	unsigned long	ulRB;					/*	Range  for b*				*/
	unsigned long	ulOB;					/*	Range  for b*				*/
	unsigned long	ulIL;					/*	Illuminant					*/

}	JP2_CIE_LAB_Params;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Color specification structure for color space  information				*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
	JP2_Colorspace		ulColourspace;		/*	Color space type			*/
	long				lPrecedence;		/*	Precedence for this spec.	*/
	unsigned long		ulApproximation;	/*	Accuracy of this spec.		*/

}	JP2_Color_Spec;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Structure for labeled XML data											*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
	char			*pcLabel;				/*	Label for the XML box.		*/
	unsigned long	ulLabelSize;			/*	Label text length.			*/
	unsigned char	*pucData;				/*	XML data.					*/
	unsigned long	ulSize;					/*	Size of XML data array.		*/

}	JP2_Labeled_XML;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Rectangle structure for decompression of regions						*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct
{
	unsigned long		ulLeft;	
	unsigned long		ulRight;
	unsigned long		ulTop;
	unsigned long		ulBottom;

}	JP2_Rect;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Version Information Function											*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------------------------------------------------------------------*/
/*																			*/
/*	Get the C-SDK version number and required license level.				*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*																			*/
/*	unsigned long		*pulVersionMajor	:	set to major version number	*/
/*	unsigned long		*pulVersionMinor	:	set minor version number	*/
/*	char				**ppcVersionString	:	set to version string		*/
/*	unsigned char		*pucLicenseLevel	:	set to minimal license		*/
/*											:	level needed for this C-SDK	*/
/*																			*/
/*	JP2 error code returned													*/
/*																			*/
/*--------------------------------------------------------------------------*/

JP2_API	JP2_Error	JP2_Common_Get_Library_Version(
										unsigned long		*pulVersionMajor,
										unsigned long		*pulVersionMinor,
										char				**ppcVersionString,
										unsigned char		*pucLicenseLevel);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	License Information Functions											*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------------------------------------------------------------------*/
/*																			*/
/*	Get the C-SDK license code evaluation expiry date.						*/
/*																			*/
/*	If the C-SDK license code is unlimited then the day, month and year		*/
/*	values are set to 0.													*/
/*																			*/
/*	Otherwise the day, month and year are set to the expiry date for the	*/
/*	evaluation license, with day ranging from 1 to 31, month from 1 to 12	*/
/*	and year from 2000 to 2200.												*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*																			*/
/*	unsigned long		ulNum1		:	first license number.				*/
/*	unsigned long		ulNum2		:	second license number.				*/
/*	unsigned char		*pucDay		:	set to expiry day.					*/
/*	unsigned char		*pucMonth	:	set to expiry month.				*/
/*	unsigned short		*pusYear	:	set to expiry year.					*/
/*																			*/
/*	JP2 error code returned													*/
/*																			*/
/*--------------------------------------------------------------------------*/

JP2_API	JP2_Error	JP2_Common_Get_License_Expiry_Date(
										unsigned long			ulNum1,
										unsigned long			ulNum2,
										unsigned char			*pucDay,
										unsigned char			*pucMonth,
										unsigned short			*pusYear);

/*--------------------------------------------------------------------------*/
/*																			*/
/*	Get the C-SDK license code level.										*/
/*																			*/
/*	A license code can only be used with libraries with an level number		*/
/*	equal to or less than the license code level. LWF_JP2_LICENSE_LEVEL		*/
/*	contains the library level.												*/
/*																			*/
/*--------------------------------------------------------------------------*/
/*																			*/
/*	unsigned long		ulNum1		:	first license number.				*/
/*	unsigned long		ulNum2		:	second license number.				*/
/*	unsigned char		*pucLevel	:	set to license code level.			*/
/*																			*/
/*	JP2 error code returned													*/
/*																			*/
/*--------------------------------------------------------------------------*/

JP2_API	JP2_Error	JP2_Common_Get_License_Level(
										unsigned long			ulNum1,
										unsigned long			ulNum2,
										unsigned char			*pucLevel);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Compression																*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/****************************************************************************/
/*	Handle																	*/
/****************************************************************************/

typedef void*			JP2_Comp_Handle;

/****************************************************************************/
/*	Exported functions														*/
/****************************************************************************/

/*	Main functions															*/

JP2_API	 JP2_Error	JP2_Compress_Start(	JP2_Comp_Handle			*pHandle,
										JP2_Malloc_Func			refMallocFunc,
										JP2_Callback_Param		lMallocParam,
										JP2_Free_Func			refFreeFunc,
										JP2_Callback_Param		lFreeParam,
										short					sComponents);

JP2_API JP2_Error	JP2_Compress_SetLicense(	
										JP2_Comp_Handle			handle,
										unsigned long			ulNum_1,
										unsigned long			ulNum_2 );

JP2_API	JP2_Error	JP2_Compress_UseExternalCache(
										JP2_Comp_Handle			handle,
										JP2_Write_Func			refWriteFunc,
										JP2_Callback_Param		lWriteParam,
										JP2_Read_Func			refReadFunc,
										JP2_Callback_Param		lReadParam);

JP2_API JP2_Error	JP2_Compress_SetProp(	
										JP2_Comp_Handle			handle,
										JP2_Property_Tag		prop,
										JP2_Property_Value		value,
										long					lTile,
										short					sComponent);

/*	Note that embedded ICC profiles may only be stored in JP2 or JPX file	*/
/*	format images. Plain codestream format images can not store ICC profile	*/
/*	data.																	*/

JP2_API	JP2_Error	JP2_Compress_SetICC(
										JP2_Comp_Handle			handle,
										unsigned char			*pucData,
										unsigned long			ulSize);

JP2_API	JP2_Error	JP2_Compress_SetLAB(
										JP2_Comp_Handle			handle,
										JP2_CIE_LAB_Params		*pLabData);

JP2_API JP2_Error	JP2_Compress_SetPalette(	
										JP2_Comp_Handle			handle,
										JP2_Palette_Params		*pPalette);

JP2_API JP2_Error	JP2_Compress_SetChannelDefs(	
										JP2_Comp_Handle			handle,
										JP2_Channel_Def_Params	*pChannels,
										unsigned long			ulNum);

JP2_API JP2_Error	JP2_Compress_GetProp(	
										JP2_Comp_Handle			handle,
										JP2_Property_Tag		prop,
										JP2_Property_Value		*plValue,
										long					lTile,
										short					sComponent);

JP2_API JP2_Error	JP2_Compress_SetFeature(	
										JP2_Comp_Handle			handle,
										JP2_Standard_Feature	feature);

JP2_API JP2_Error	JP2_Compress_SetPrecinct(	
										JP2_Comp_Handle			handle,
										unsigned long			ulPPx,
										unsigned long			ulPPy,
										long					lTile,
										short					sComponent,
										long					sLevel);

/*	Note that resolution information may only be stored in JP2 or JPX		*/
/*	file format	images.	 Plain codestream format images can not store		*/
/*	resolution information.													*/

JP2_API JP2_Error	JP2_Compress_SetResolution(
										JP2_Comp_Handle			handle,
										unsigned long			ulVR,
										unsigned long			ulHR,
										JP2_DPM_Resolution		*psDPM,
										JP2_Resolution_Units	ulResUnit,
										JP2_Resolution_Types	ulResType );

JP2_API JP2_Error	JP2_Compress_SetROI(
										JP2_Comp_Handle			handle,
										unsigned long			ul_ROI_X,
										unsigned long			ul_ROI_Y,
										unsigned long			ul_ROI_Width,
										unsigned long			ul_ROI_Height );

JP2_API JP2_Error	JP2_Compress_SetROI_Enhance(
										JP2_Comp_Handle			handle,
										unsigned long			ul_ROI_Enhance );

JP2_API JP2_Error	JP2_Compress_Image(	JP2_Comp_Handle			handle);

JP2_API JP2_Error	JP2_Compress_End(	JP2_Comp_Handle			handle);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Note that XML, IP, UUID and UUID Info data may only be stored when		*/
/*	using the JP2 or JPX file format. The plain codestream format can not	*/
/*	store these types of information.										*/
/*																			*/
/*	If XML, IP, UUID or UUID Info is set after calling JP2_Compress_Image	*/
/*	then the information will be added to the stream after the coded image	*/
/*	data and the additional XML, IP, UUID or UUID_Info information may		*/
/*	result in the file size being larger than the requested size.			*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

JP2_API JP2_Error	JP2_Compress_SetXML_Data(
										JP2_Comp_Handle			handle,
										unsigned char			*pucData,
										unsigned long			ulSize);

JP2_API JP2_Error	JP2_Compress_SetLabeledXML_Data(
										JP2_Comp_Handle			handle,
										JP2_Labeled_XML			*pData);

JP2_API JP2_Error	JP2_Compress_SetLabeledXMLCollection_Data(
										JP2_Comp_Handle			handle,
										char					*pcLabel,
										unsigned long			ulLabelSize,
										JP2_Labeled_XML			*pData,
										unsigned long			ulSize);
										
JP2_API JP2_Error	JP2_Compress_SetIP_Data(
										JP2_Comp_Handle			handle,
										unsigned char			*pucData,
										unsigned long			ulSize);

JP2_API JP2_Error	JP2_Compress_SetUUID_Data(
										JP2_Comp_Handle			handle,
										JP2_UUID				*pUUID,
										unsigned char			*pucData,
										unsigned long			ulSize);

JP2_API JP2_Error	JP2_Compress_SetUUID_Info(
										JP2_Comp_Handle			handle,
										JP2_UUID				*pUUID_List,
										unsigned short			usEntries,
										char					*pcURL,
										unsigned long			ulSize);

JP2_API	JP2_Error	JP2_Compress_SetIPTC_Data(
										JP2_Comp_Handle			handle,
										unsigned char			*pucData,
										unsigned long			ulSize);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Decompression															*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/****************************************************************************/
/*	Handle																	*/
/****************************************************************************/

typedef void*			JP2_Decomp_Handle;

/****************************************************************************/
/*	Exported functions														*/
/****************************************************************************/

JP2_API JP2_Error	JP2_Decompress_Start(	
										JP2_Decomp_Handle		*pHandle,
										JP2_Malloc_Func			refMallocFunc,
										JP2_Callback_Param		lMallocParam,
										JP2_Free_Func			refFreeFunc,
										JP2_Callback_Param		lFreeParam,
										JP2_Read_Func			refReadFunc,
										JP2_Callback_Param		lReadParam);
									
JP2_API JP2_Error	JP2_Decompress_SetLicense(	
										JP2_Decomp_Handle		handle,
										unsigned long			ulNum_1,
										unsigned long			ulNum_2 );

JP2_API JP2_Error	JP2_Decompress_SetProp(	
										JP2_Decomp_Handle		handle,
										JP2_Property_Tag		prop,
										JP2_Property_Value		value);

JP2_API JP2_Error	JP2_Decompress_GetProp(	
										JP2_Decomp_Handle		handle,
										JP2_Property_Tag		prop,
										JP2_Property_Value		*plValue,
										long					lTile,
										short					sComponent);

JP2_API	JP2_Error	JP2_Decompress_GetColorSpec(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum,
										JP2_Color_Spec			**ppCSpec_List);
										
JP2_API	JP2_Error	JP2_Decompress_SetColorSpec(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex);
										
JP2_API JP2_Error	JP2_Decompress_GetICC(	
										JP2_Decomp_Handle		handle,
										unsigned char			**ppucData,
										unsigned long			*pulSize);

JP2_API	JP2_Error	JP2_Decompress_GetLAB(
										JP2_Decomp_Handle		handle,
										JP2_CIE_LAB_Params		*pLabData);

JP2_API JP2_Error	JP2_Decompress_GetPalette(	
										JP2_Decomp_Handle		handle,
										JP2_Palette_Params		**ppPalette);

JP2_API JP2_Error	JP2_Decompress_GetChannelDefs(	
										JP2_Decomp_Handle		handle,
										JP2_Channel_Def_Params	**ppChannels,
										unsigned long			*pulNum);

JP2_API JP2_Error	JP2_Decompress_GetPrecinct(	
										JP2_Decomp_Handle		handle,
										unsigned long			*pulPPx,
										unsigned long			*pulPPy,
										long					lTile,
										short					sComponent,
										long					sLevel);

JP2_API JP2_Error	JP2_Decompress_GetResolution(	
										JP2_Decomp_Handle		handle,
										unsigned long			*pulVR,
										unsigned long			*pulHR,
										JP2_DPM_Resolution		*psDPM,
										JP2_Resolution_Units	ulResUnit,
										JP2_Resolution_Types	ulResType);

/*	*pulFlag set to 1 if a reader requirements box indicates that the		*/
/*	standard feature is contained in the file, otherwise *pulFlag set to 0.	*/

JP2_API JP2_Error	JP2_Decompress_ContainsFeature(	
										JP2_Decomp_Handle		handle,
										JP2_Standard_Feature	feature,
										unsigned long			*pulFlag);

JP2_API JP2_Error	JP2_Decompress_Image(
										JP2_Decomp_Handle		handle);

JP2_API JP2_Error	JP2_Decompress_End(	
										JP2_Decomp_Handle		handle);

JP2_API JP2_Error	JP2_Decompress_GetCOM_Num(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum );

JP2_API JP2_Error	JP2_Decompress_GetCOM_Data(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										unsigned short			*pusReg,
										unsigned char			**ppucData,
										unsigned long			*pulSize );

JP2_API JP2_Error	JP2_Decompress_GetXML_Num(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum );

JP2_API JP2_Error	JP2_Decompress_GetXML_Data(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										unsigned char			**ppucData,
										unsigned long			*pulSize );

JP2_API JP2_Error   JP2_Decompress_GetLabeledXML_Data(
                                        JP2_Decomp_Handle       handle,
                                        char                    *pcLabel,
                                        unsigned long           ulLabelSize,
                                        unsigned char           **ppucData,
										unsigned long			*pulSize );

JP2_API JP2_Error	JP2_Decompress_GetIP_Num(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum );

JP2_API JP2_Error	JP2_Decompress_GetIP_Data(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										unsigned char			**ppucData,
										unsigned long			*pulSize );

JP2_API JP2_Error	JP2_Decompress_GetUUID_Num(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum);

JP2_API JP2_Error	JP2_Decompress_GetUUID_Id(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										JP2_UUID				*pUUID);

JP2_API JP2_Error	JP2_Decompress_GetUUID_Data(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										unsigned char			**ppucData,
										unsigned long			*pulSize );

JP2_API JP2_Error	JP2_Decompress_GetUUID_Info_Num(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum );

JP2_API JP2_Error	JP2_Decompress_GetUUID_Info_Data(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										JP2_UUID				**ppUUID_List,
										unsigned short			*pusEntries,
										char					**ppcURL,
										unsigned long			*pulSize);

JP2_API JP2_Error	JP2_Decompress_GetIPTC_Num(
										JP2_Decomp_Handle		handle,
										unsigned long			*pulNum);

JP2_API	JP2_Error	JP2_Decompress_GetIPTC_Data(
										JP2_Decomp_Handle		handle,
										unsigned long			ulIndex,
										unsigned char			**ppucData,
										unsigned long			*pulSize);

/****************************************************************************/
/*	Region decompression functions	(former geo edition functions)			*/
/****************************************************************************/

JP2_API JP2_Error	JP2_Decompress_GetComponentRegion(	
										JP2_Decomp_Handle		handle,
										JP2_Rect				region,
										JP2_Rect				*comp_region,
										short					sComponent);

JP2_API JP2_Error	JP2_Decompress_GetTileComponentRegion(	
										JP2_Decomp_Handle		handle,
										unsigned long			ulTile,
										JP2_Rect				*comp_region,
										short					sComponent);

JP2_API JP2_Error	JP2_Decompress_Region(
										JP2_Decomp_Handle		handle,
										JP2_Rect				region);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Transcoding																*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/****************************************************************************/
/*	Handle																	*/
/****************************************************************************/

typedef void*			JP2_Trans_Handle;

/****************************************************************************/
/*	Exported functions														*/
/****************************************************************************/

JP2_API JP2_Error	JP2_Transcode_Start(	
										JP2_Trans_Handle		*pHandle,
										JP2_Malloc_Func			refMallocFunc,
										JP2_Callback_Param		lMallocParam,
										JP2_Free_Func			refFreeFunc,
										JP2_Callback_Param		lFreeParam,
										JP2_Read_Func			refReadFunc,
										JP2_Callback_Param		lReadParam);
									
JP2_API JP2_Error	JP2_Transcode_SetLicense(	
										JP2_Trans_Handle		handle,
										unsigned long			ulNum_1,
										unsigned long			ulNum_2 );

JP2_API JP2_Error	JP2_Transcode_SetProp(	
										JP2_Trans_Handle		handle,
										JP2_Property_Tag		prop,
										JP2_Property_Value		value,
										long					lTile);

JP2_API JP2_Error	JP2_Transcode_GetProp(	
										JP2_Trans_Handle		handle,
										JP2_Property_Tag		prop,
										JP2_Property_Value		*plValue,
										long					lTile,
										short					sComponent);

JP2_API JP2_Error	JP2_Transcode_Image(JP2_Trans_Handle		handle);

JP2_API JP2_Error	JP2_Transcode_End(	JP2_Trans_Handle		handle);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	Note that XML, IP, UUID and UUID Info data may only be stored when		*/
/*	using the JP2 or JPX file format. The plain codestream format can not	*/
/*	store these types of information.										*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

JP2_API JP2_Error	JP2_Transcode_SetXML_Data(	
										JP2_Trans_Handle		handle,
										unsigned char*			pucData,
										unsigned long			ulSize);

JP2_API JP2_Error	JP2_Transcode_SetLabeledXML_Data(
										JP2_Comp_Handle			handle,
										JP2_Labeled_XML			*pData);

JP2_API JP2_Error	JP2_Transcode_SetLabeledXMLCollection_Data(
										JP2_Comp_Handle			handle,
										char					*pcLabel,
										unsigned long			ulLabelSize,
										JP2_Labeled_XML			*pData,
										unsigned long			ulSize);
										
JP2_API JP2_Error	JP2_Transcode_SetIP_Data(
										JP2_Trans_Handle		handle,
										unsigned char			*pucData,
										unsigned long			ulSize);

JP2_API JP2_Error	JP2_Transcode_SetUUID_Data(
										JP2_Trans_Handle		handle,
										JP2_UUID				*pUUID,
										unsigned char			*pucData,
										unsigned long			ulSize);

JP2_API JP2_Error	JP2_Transcode_SetUUID_Info(
										JP2_Trans_Handle		handle,
										JP2_UUID				*pUUID_List,
										unsigned short			usEntries,
										char					*pcURL,
										unsigned long			ulSize);

JP2_API	JP2_Error	JP2_Transcode_SetIPTC_Data(
										JP2_Trans_Handle		handle,
										unsigned char			*pucData,
										unsigned long			ulSize);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*																			*/
/*	General																	*/
/*																			*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/****************************************************************************/
/*	Exported functions														*/
/****************************************************************************/

JP2_API JP2_Error	JP2_General_CheckICC(	
										unsigned char			*pucData,
										unsigned long			ulSize,
										unsigned long			*pulJP2_Compatible,
										unsigned long			*pulRestricted,
										unsigned long			*pulComponents);

#ifdef __cplusplus
    }
#endif

#endif
