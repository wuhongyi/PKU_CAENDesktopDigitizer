// note.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 11月 17 21:16:52 2016 (+0800)
// Last-Updated: 五 11月 18 10:56:21 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

typedef struct {
    char                        ModelName[12];
    uint32_t                    Model;
    uint32_t                    Channels;
    uint32_t                    FormFactor;
    uint32_t                    FamilyCode;
    char                        ROC_FirmwareRel[20];
    char                        AMC_FirmwareRel[40];
    uint32_t                    SerialNumber;
    char                        MezzanineSerNum[4][8];       //used only for x743 boards
    uint32_t                    PCB_Revision;
    uint32_t                    ADC_NBits;
    uint32_t                    SAMCorrectionDataLoaded;        //used only for x743 boards
    int                         CommHandle;
    int                         VMEHandle;
    char                        License[MAX_LICENSE_LENGTH];
} CAEN_DGTZ_BoardInfo_t;

typedef struct 
{
    uint32_t             EventSize;
    uint32_t             BoardId;
    uint32_t             Pattern;
    uint32_t             ChannelMask;
    uint32_t             EventCounter;
    uint32_t             TriggerTimeTag;
} CAEN_DGTZ_EventInfo_t;

typedef enum {
    CAEN_DGTZ_TriggerOnRisingEdge        = 0L,
    CAEN_DGTZ_TriggerOnFallingEdge        = 1L,
} CAEN_DGTZ_TriggerPolarity_t;

typedef enum {
    CAEN_DGTZ_PulsePolarityPositive        = 0,
    CAEN_DGTZ_PulsePolarityNegative        = 1,
} CAEN_DGTZ_PulsePolarity_t;


/*! 
 * \brief Defines the kind of histogram data returned in readout data
 */
typedef enum
{
    CAEN_DGTZ_DPP_SAVE_PARAM_EnergyOnly     = 0L, /*!< Only energy (DPP-PHA) or charge (DPP-PSD/DPP-CI v2) is returned */
    CAEN_DGTZ_DPP_SAVE_PARAM_TimeOnly       = 1L, /*!< Only time is returned */
    CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime  = 2L, /*!< Both energy/charge and time are returned */
    CAEN_DGTZ_DPP_SAVE_PARAM_ChargeAndTime  = 4L, /*!< \deprecated On DPP-PSD and DPP-CI use CAEN_DGTZ_DPP_SAVE_PARAM_EnergyAndTime */
    CAEN_DGTZ_DPP_SAVE_PARAM_None           = 3L, /*!< No histogram data is returned */
} CAEN_DGTZ_DPP_SaveParam_t;


typedef enum CAEN_DGTZ_DPP_PARAMETER
{
    CAEN_DGTZ_DPP_Param_m                = 0L,
    CAEN_DGTZ_DPP_Param_M                = 1L,
    CAEN_DGTZ_DPP_Param_Delta1            = 2L,
    CAEN_DGTZ_DPP_Param_a                = 3L,
    CAEN_DGTZ_DPP_Param_b                = 4L,
    CAEN_DGTZ_DPP_Param_NSBaseline        = 5L,
    CAEN_DGTZ_DPP_Param_shf                = 6L,
    CAEN_DGTZ_DPP_Param_k                = 7L,
    CAEN_DGTZ_DPP_Param_NSPeakMean        = 8L,
    CAEN_DGTZ_DPP_Param_FlatTopDelay    = 9L,
    CAEN_DGTZ_DPP_Param_Decimation        = 10L,
    CAEN_DGTZ_DPP_Param_TrgThreshold    = 11L,
    CAEN_DGTZ_DPP_Param_TrgWinOffset    = 12L,
    CAEN_DGTZ_DPP_Param_TrgWinWidth        = 13L,
    CAEN_DGTZ_DPP_Param_DigitalGain        = 14L,
    CAEN_DGTZ_DPP_Param_GateWidth        = 15L,
    CAEN_DGTZ_DPP_Param_PreGate            = 16L,
    CAEN_DGTZ_DPP_Param_HoldOffTime        = 17L,
    CAEN_DGTZ_DPP_Param_BslThreshold    = 18L,
    CAEN_DGTZ_DPP_Param_NoFlatTime        = 19L,
    CAEN_DGTZ_DPP_Param_GateMode        = 20L,
    CAEN_DGTZ_DPP_Param_InvertInput        = 21L,
} CAEN_DGTZ_DPP_PARAMETER_t;


/*! 
 * \brief DPP parameter structure to be initialized and passed to CAEN_DGTZ_SetDPPParameters
 * \note Only for DPP-PSD
 */
typedef struct {
    int blthr;
    int bltmo;
    int trgho;
    int thr         [MAX_DPP_PSD_CHANNEL_SIZE];
    int selft       [MAX_DPP_PSD_CHANNEL_SIZE];
    int csens       [MAX_DPP_PSD_CHANNEL_SIZE];
    int sgate       [MAX_DPP_PSD_CHANNEL_SIZE];
    int lgate       [MAX_DPP_PSD_CHANNEL_SIZE];
    int pgate       [MAX_DPP_PSD_CHANNEL_SIZE];
    int tvaw        [MAX_DPP_PSD_CHANNEL_SIZE];
    int nsbl        [MAX_DPP_PSD_CHANNEL_SIZE];
    CAEN_DGTZ_DPP_TriggerConfig_t trgc // Ignored for x751
                    [MAX_DPP_PSD_CHANNEL_SIZE];
    CAEN_DGTZ_DPP_PUR_t purh; // Ignored for x751
    int purgap; // Ignored for x751
} CAEN_DGTZ_DPP_PSD_Params_t;


/*! 
 * \brief Defines the trigger mode to be set with CAEN_DGTZ_SetDPPTriggerMode
 * \note Only for DPP-PSD and DPP-CI version 2
 */
typedef enum
{
    CAEN_DGTZ_DPP_TriggerMode_Normal,
    CAEN_DGTZ_DPP_TriggerMode_Coincidence
} CAEN_DGTZ_DPP_TriggerMode_t;


/*! 
 * \brief Defines the trigger configuration to be set with CAEN_DGTZ_SetDPPTriggerConfig
 * \note Only for DPP-PSD and DPP-CI version 2
 */
typedef enum
{
    CAEN_DGTZ_DPP_TriggerConfig_Peak,
    CAEN_DGTZ_DPP_TriggerConfig_Threshold
} CAEN_DGTZ_DPP_TriggerConfig_t;


/*! 
 * \brief Waveform type for DPP-PSD to be used within the <b>new readout API</b>
 */
typedef struct
{
    uint32_t Ns;
    uint8_t  dualTrace;
    uint8_t  anlgProbe;
    uint8_t  dgtProbe1;
    uint8_t  dgtProbe2;
    uint16_t *Trace1;
    uint16_t *Trace2;
    uint8_t  *DTrace1;
    uint8_t  *DTrace2;
    uint8_t  *DTrace3;
    uint8_t  *DTrace4;
} CAEN_DGTZ_DPP_PSD_Waveforms_t;


/*! 
 * \brief Event type for DPP-PSD to be used within the <b>new readout API</b>
 */
typedef struct 
{
    uint32_t Format;
    uint32_t TimeTag;
    int16_t ChargeShort;
    int16_t ChargeLong;
    int16_t Baseline;
    int16_t Pur;
    uint32_t *Waveforms; /*!< pointer to coded data inside the readout buffer. only meant to be supplied to CAEN_DGTZ_DecodeDPPWaveforms */ 
    uint32_t Extras;
} CAEN_DGTZ_DPP_PSD_Event_t;


/*! 
 * \brief Defines the signals that can be carried by the virtual analog probe
 *        in the readout data of the DPP-PSD
 */
typedef enum
{
    CAEN_DGTZ_DPP_PSD_VIRTUALPROBE_Baseline     = 0L,
    CAEN_DGTZ_DPP_PSD_VIRTUALPROBE_Threshold    = 1L,
} CAEN_DGTZ_DPP_PSD_VirtualProbe_t;

/*! 
 * \brief Defines the signals that can be carried by the digital probe 1
 *        in the readout data of the DPP-PSD
 */
typedef enum
{
    /************************************************************
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    *  The following values are valid for the following DPP-PSD *
    *  Firmwares:                                               *
    *       x720 Boards: AMC_REL <= 131.5                       *
    *       x751 Boards: AMC_REL <= 132.5                       *
    *  For newer firmwares, use the values marked with 'R6' in  *
    *  the name.                                                *
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    ************************************************************/
    
    /* x720 Digital Probes Types */
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_Armed           = 0L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_Trigger         = 1L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_ChargeReady     = 2L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_PileUp          = 3L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_BlOutSafeBand   = 4L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_BlTimeout       = 5L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_CoincidenceMet  = 6L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_Tvaw            = 7L,

    /* x751 Digital Probes Types */
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_OverThr         = 8L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_GateShort       = 9L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_None            = 10L,

    /************************************************************
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    *  The following values are valid for the following DPP-PSD *
    *  Firmwares:                                               *
    *       x720 Boards: AMC_REL >= 131.6                       *
    *       x751 Boards: AMC_REL >= 132.6                       *
    *  For older firmwares, use the values above.               *
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    ************************************************************/
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_ExtTrg       = 11L, /* x720 only */
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_OverThr      = 12L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_TrigOut      = 13L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_CoincWin     = 14L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_PileUp       = 15L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_Coincidence  = 16L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE1_R6_GateLong     = 17L, /* x751 only */ 
} CAEN_DGTZ_DPP_PSD_DigitalProbe1_t;

/*! 
 * \brief Defines the signals that can be carried by the digital probe 2
 *        in the readout data of the DPP-PSD
 */
typedef enum
{
    /************************************************************
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    *  The following values are valid for the following DPP-PSD *
    *  Firmwares:                                               *
    *       x720 Boards: AMC_REL <= 131.5                       *
    *       x751 Boards: AMC_REL <= 132.5                       *
    *  For newer firmwares, use the values marked with 'R6' in  *
    *  the name.                                                *
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    ************************************************************/

    /* x720 Digital Probes Types */
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_Armed           = 0L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_Trigger         = 1L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_ChargeReady     = 2L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_PileUp          = 3L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_BlOutSafeBand   = 4L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_BlTimeout       = 5L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_CoincidenceMet  = 6L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_Tvaw            = 7L,

    /* x751 Digital Probes Types */
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_GateShort       = 8L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_GateLong        = 9L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_None            = 10L,

    /************************************************************
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    *  The following values are valid for the following DPP-PSD *
    *  Firmwares:                                               *
    *       x720 Boards: AMC_REL >= 131.6                       *
    *       x751 Boards: AMC_REL >= 132.6                       *
    *  For older firmwares, use the values above.               *
    *  WARNING WARNING WARNING WARNING WARNING WARNING WARNING  *
    ************************************************************/
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_GateShort    = 11L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_OverThr      = 12L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_TrgVal       = 13L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_TrgHO        = 14L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_PileUp       = 15L,
    CAEN_DGTZ_DPP_PSD_DIGITALPROBE2_R6_Coincidence  = 16L,
} CAEN_DGTZ_DPP_PSD_DigitalProbe2_t;


typedef enum {
    CAEN_DGTZ_DPP_Channel_0                    = 0L,
    CAEN_DGTZ_DPP_Channel_1                    = 1L,
    CAEN_DGTZ_DPP_Channel_2                    = 2L,
    CAEN_DGTZ_DPP_Channel_3                    = 3L,
    CAEN_DGTZ_DPP_Channel_4                    = 4L,
    CAEN_DGTZ_DPP_Channel_5                    = 5L,
    CAEN_DGTZ_DPP_Channel_6                    = 6L,
    CAEN_DGTZ_DPP_Channel_7                    = 7L,
    CAEN_DGTZ_DPP_Channel_ALL                  = 255L,
} CAEN_DGTZ_DPP_Channel_t;

/*! 
 * \brief Defines whether to include an additional virtual analog probe in the readout data
 *        at the cost of halving the resolution of the first
 */
typedef enum
{
    CAEN_DGTZ_DPP_VIRTUALPROBE_SINGLE        = 0L,
    CAEN_DGTZ_DPP_VIRTUALPROBE_DUAL          = 1L,
} CAEN_DGTZ_DPP_VirtualProbe_t;


typedef enum 
{
    CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT        = 0L,
    CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eVME     = 1L,
    CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eSST     = 2L,
    CAEN_DGTZ_POLLING_MBLT                         = 3L,
    CAEN_DGTZ_POLLING_2eVME                        = 4L,
    CAEN_DGTZ_POLLING_2eSST                        = 5L,
} CAEN_DGTZ_ReadMode_t;

typedef enum
{
    CAEN_DGTZ_DPP_ACQ_MODE_Oscilloscope            = 0L,
    CAEN_DGTZ_DPP_ACQ_MODE_List                    = 1L,
    CAEN_DGTZ_DPP_ACQ_MODE_Mixed                = 2L,
} CAEN_DGTZ_DPP_AcqMode_t;

















/**************************************************************************//**
* \fn      CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress, int *handle);
* \brief   Opens the Digitizer
*
* \param   [IN]  LinkType      : The link used by the device
* \param   [IN]  LinkNum:
*                         - when using CONET, it is the optical link number to be used
*                         - when using USB, it is the USB device number to be used
* \param   [IN]  ConetNode     :
*                         - for CONET identify  which device in the daisy-chain is addressed
*                         - for USB must be 0.
* \param   [IN]     VMEBaseAddress: The VME base address of the board in case you want to access a board through VME bus, 0 otherwise.                         
* \param   [OUT] handle        : device handler
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress, int *handle);


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_CloseDigitizer(int handle);
* \brief     Closes the Digitizer
*
* \param     [IN] handle: digitizer handle to be closed
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_CloseDigitizer(int handle);


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetInfo(int handle, CAEN_DGTZ_BoardInfo_t *BoardInfo)
* \brief     Retrieves the board information of the digitizer
*
* \param     [IN] handle     : the digitizer handle
* \param    [OUT] BoardInfo : the CAEN_DGTZ_BoardInfo_t with the board information retrieved from the device
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetInfo(int handle, CAEN_DGTZ_BoardInfo_t *BoardInfo); 


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_Reset(int handle);
* \brief     Resets the Digitizer. All internal registers and states are restored
*           to defaults.
*
* \param     [IN] handle: digitizer handle to be reset
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_Reset(int handle);


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_ClearData(int handle);
* \brief     Clears the data stored in the buffers of the Digitizer
*
* \param     [IN] handle: digitizer handle.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_ClearData(int handle);


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SendSWtrigger(int handle);
* \brief     Sends a Software trigger to the Digitizer
*
* \param     [IN] handle: digitizer handle.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SendSWtrigger(int handle);


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SWStartAcquisition(int handle);
* \brief     Starts Digitizers acquisition.
*
* \param     [IN] handle: digitizer handle.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SWStartAcquisition(int handle);


/**************************************************************************//**
* \fn          CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SWStopAcquisition(int handle);
* \brief     Stops Digitizer acquisition 
*
* \param     [IN] handle: digitizer handle.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SWStopAcquisition(int handle);


/**************************************************************************//**
* \brief     Sets Dual Edge Sampling (DES) mode. Valid only for digitizers that supports this acquisiton mode
*
* \param     [IN] handle : digitizer handle
* \param     [IN] enable : Enable/Disable Dual Edge Sampling mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDESMode(int handle, CAEN_DGTZ_EnaDis_t enable);


/**************************************************************************//**
* \brief     Gets Dual Edge Sampling (DES) mode. Valid only for digitizers that supports this acquisiton mode.
*
* \param     [IN] handle  : digitizer handle
* \param     [OUT] enable : shows current DES mode status (enabled/disabled)
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDESMode(int handle, CAEN_DGTZ_EnaDis_t *enable);


/**************************************************************************//**
* \brief     Sets acquisition record length
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] size    : the size of the record (in samples)
* \param    [IN] channel : \c int specifying the channel to set the record length for. 
*                           DPP-PSD and DPP-CI require this parameter,
*                           DPP-PHA ignores it
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetRecordLength(int handle, uint32_t size, ...);


/**************************************************************************//**
* \brief     Gets current acquisition record length
*
* \param     [IN] handle  : digitizer handle
* \param     [OUT] size   : the size of the record (in samples)
* \param    [IN] channel : \c int specifying the channel to get the record length for. 
*                           DPP-PSD and DPP-CI require this parameter,
*                           DPP-PHA ignores it
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetRecordLength(int handle, uint32_t *size, ...);


/**************************************************************************//**
* \brief     Sets channels that will be  enabled into events
*
* \param     [IN] handle : digitizer handle.
* \param     [IN] mask   : enabled channels mask.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelEnableMask(int handle, uint32_t mask);


/**************************************************************************//**
* \brief     Gets current mask of enabled channels in events.
*           If a mask bit is set, corresponding channel is currenly enabled for event readout
*
* \param     [IN] handle : digitizer handle.
* \param     [OUT] mask  : enabled channels mask.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelEnableMask(int handle, uint32_t *mask);


/**************************************************************************//**
* \brief     Sets oneof the available trigger mode
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode   : trigger mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetSWTriggerMode(int handle, CAEN_DGTZ_TriggerMode_t mode);


/**************************************************************************//**
* \brief     Sets external trigger input mode
*
* \param     [IN] handle : digitizer handle.
* \param     [IN] mode   : external trigger input mode.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetExtTriggerInputMode(int handle, CAEN_DGTZ_TriggerMode_t mode);


/**************************************************************************//**
* \brief     Gets current external trigger input mode
*
* \param     [IN] handle : digitizer handle.
* \param     [OUT] mode  : extrnal input trigger mode.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetExtTriggerInputMode(int handle, CAEN_DGTZ_TriggerMode_t *mode);


/**************************************************************************//**
* \brief     Sets software trigger mode
*
* \param     [IN] handle : digitizer handle
* \param     [OUT] mode  : trigger mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetSWTriggerMode(int handle, CAEN_DGTZ_TriggerMode_t *mode);


/**************************************************************************//**
* \brief     Sets channel self trigger mode according to CAEN_DGTZ_TriggerMode_t. 
*
* \param     [IN] handle      : digitizer handle
* \param     [IN] mode        : self trigger mode
* \param    [IN] channelmask : channel mask to select affected channels.
* \return  0 = Success; negative numbers are error codes
*
* NOTE: since x730 board family has even and odd channels paired, the user
*  shouldn't call this function separately for the channels of the same pair,
*  otherwise the second call will overwrite the setting of the first one. The
*  user should instead call at maximum once for every pair with the relevant
*  bits of the channelmask already set to the correct value.
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelSelfTrigger(int handle, CAEN_DGTZ_TriggerMode_t mode, uint32_t channelmask);


/**************************************************************************//**
* \brief     Gets current channel self trigger mode setting. 
*
* \param     [IN] handle  : digitizer handle
* \param    [IN] channel : select channel to get the self trigger mode
* \param     [OUT] mode   : current trigger mode for selected channel
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelSelfTrigger(int handle, uint32_t channel, CAEN_DGTZ_TriggerMode_t *mode);


/**************************************************************************//**
* \brief     Sets post trigger for next acquisitions
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] percent : the percent of current record length
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetPostTriggerSize(int handle, uint32_t percent);


/**************************************************************************//**
* \brief     Gets current post trigger length
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] percent : the percent of the record
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetPostTriggerSize(int handle, uint32_t *percent);


/**************************************************************************//**
* \brief     Sets the pre-trigger size, which is the portion of acquisition window visible before a trigger
* \note        This function is only available to DPP enabled firmwares and only to DPP-PHA, DPP-PSD and DPP-CI
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] ch      : the channel to set the pre-trigger for. ch=-1 writes the value for all channels.
                           DPP-CI only supports ch=-1 (different channels must have the same pre-trigger)
* \param     [IN] sample  : the pre-trigger size, in samples
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPPreTriggerSize(int handle, int ch, uint32_t samples);


/**************************************************************************//**
* \brief     Gets the pre-trigger size
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] ch      : the channel to get the pre-trigger of
* \param     [OUT] sample : the pre-trigger size, in samples
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPPPreTriggerSize(int handle, int ch, uint32_t *samples);


/**************************************************************************//**
* \brief     Sets the DC offset for a specified channel
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] channel : the channel to set
* \param    [IN] Tvalue  : the DC offset to set. Tvalue is expressed in channel DAC (Digital to Analog Converter) steps.
*                          Please refer to digitizer documentation for possible value range.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelDCOffset(int handle, uint32_t channel, uint32_t Tvalue);


/**************************************************************************//**
* \brief     Gets the DC offset for a specified channel
*
* \param     [IN]  handle  : digitizer handle.
* \param     [IN]  channel : the channel which takes the information.
* \param    [OUT] Tvalue  : the DC offset set.  
*                           Tvalue is expressed in channel DAC (Digital to Analog Converter) steps.
*                           Please refer to digitizer documentation for possible value range.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelDCOffset(int handle, uint32_t channel, uint32_t *Tvalue);


/**************************************************************************//**
* \brief     Sets the Trigger Threshold for a specific channel
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] channel : channel to set
* \param    [IN] Tvalue  : threshold value to set
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelTriggerThreshold(int handle, uint32_t channel, uint32_t Tvalue);


/**************************************************************************//**
* \brief     Gets current Trigger Threshold from a specified channel
*
* \param     [IN]  handle  : digitizer handle.
* \param     [IN]  channel : the channel which takes the information.
* \param    [OUT] Tvalue  : the threshold value set
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelTriggerThreshold(int handle, uint32_t channel, uint32_t *Tvalue);


/**************************************************************************//**
* \brief     Set the pulse polarity for the specified channel
*
* \param     [IN] handle  : digitizer handle.
* \param     [IN] channel : channel to set.
* \param    [IN] pol     : the value of the pulse polarity
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetChannelPulsePolarity(int handle, uint32_t channel, CAEN_DGTZ_PulsePolarity_t pol);


/**************************************************************************//**
* \brief     Get the value of the pulse polarity for the specified channel
*
* \param     [IN] handle  : digitizer handle.
* \param     [IN] channel : channel to get information from
* \param    [OUT] pol    : the value of the pulse polarity
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetChannelPulsePolarity(int handle, uint32_t channel, CAEN_DGTZ_PulsePolarity_t* pol);


/**************************************************************************//**
* \brief     Sets digitizer acquisition mode
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode   : acquisition mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetAcquisitionMode(int handle, CAEN_DGTZ_AcqMode_t mode);


/**************************************************************************//**
* \brief     Gets the acquisition mode of the digitizer 
*
* \param     [IN]  handle : digitizer handle
* \param     [OUT] mode   : the acquisition mode set
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetAcquisitionMode(int handle, CAEN_DGTZ_AcqMode_t *mode);


/**************************************************************************//**
* \brief     Sets waveform to output on Digitizer Analog Monitor Front Panel output 
*
* \param     [IN] handle : digitizer handle.
* \param     [IN] mode   : Analog Monitor mode.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetAnalogMonOutput(int handle, CAEN_DGTZ_AnalogMonitorOutputMode_t mode);


/**************************************************************************//**
* \brief     Gets current waveform selected to drive Digitizer Analog Monitor Front Panel output 
*
* \param     [IN]  handle : digitizer handle
* \param     [OUT] mode   : Analog Monitor output mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetAnalogMonOutput(int handle, CAEN_DGTZ_AnalogMonitorOutputMode_t *mode);


/**************************************************************************//**
* \brief     Sets max aggregate number for each transfer
* \On DPP-PHA, DPP-PSD and DPP-CI you can use CAEN_DGTZ_SetDPPEventAggregation
*
* \param     [IN] handle    : digitizer handle
* \param     [IN] numAggr : Maximum Event Number for transfer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetMaxNumAggregatesBLT(int handle, uint32_t numAggr);


/**************************************************************************//**
* \brief     Sets max event number for each transfer
* \deprecated On DPP-PHA, DPP-PSD and DPP-CI use CAEN_DGTZ_SetDPPEventAggregation
*
* \param     [IN] handle    : digitizer handle
* \param     [IN] numEvents : Maximum Event Number for transfer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetMaxNumEventsBLT(int handle, uint32_t numEvents);


/**************************************************************************//**
* \brief     Gets the max number of aggregates of each block transfer  
*
* \param     [IN]  handle    : digitizer handle
* \param     [OUT] numEvents : the number of aggregates set.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetMaxNumAggregatesBLT(int handle, uint32_t *numAggr);


/**************************************************************************//**
* \brief     Gets the max number of events of each block transfer
* \deprecated On DPP-PHA, DPP-PSD and DPP-CI use CAEN_DGTZ_SetDPPEventAggregation         
*
* \param     [IN]  handle    : digitizer handle
* \param     [OUT] numEvents : the number of events set.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetMaxNumEventsBLT(int handle, uint32_t *numEvents);


/**************************************************************************//**
* \brief     Allocates memory buffer to hold data received from digitizer.
* \note     Grandfathered into the <b>new readout API</b>
*
* \param     [IN]  handle : digitizer handle
* \param     [OUT] buffer : the address of the buffer pointer (WARNING: the *buffer MUST be initialized to NULL)
* \param     [OUT] size   : the size in byte of the buffer allocated
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_MallocReadoutBuffer(int handle, char **buffer, uint32_t *size);


/**************************************************************************//**
* \brief     Reads data (events) from the digitizer.
* \note        Grandfathered into the <b>new readout API</b>
*
* \param     [IN]  handle     : digitizer handle
* \param     [OUT] buffer     : address of the buffer that will store data (acquisition buffer)
* \param     [OUT] bufferSize : the size of the data stored in the buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_ReadData(int handle, CAEN_DGTZ_ReadMode_t mode, char *buffer, uint32_t *bufferSize);


/**************************************************************************//**
* \brief     Frees memory allocated by the CAEN_DGTZ_MallocReadoutBuffer.
* \note     Grandfathered into the <b>new readout API</b>
*
* \param     [IN] handle : digitizer handle
* \param     [IN] buffer : address to the acquisition buffer returned by CAEN_DGTZ_MallocReadoutBuffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeReadoutBuffer(char **buffer);


/**************************************************************************//**
* \brief     Gets current number of event stored in the acquisition buffer.
* \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
*
* \param     [IN] handle     : digitizer handle
* \param     [IN] buffer     : Address to the acquisition buffer 
* \param     [IN] bufferSize : Size of the data stored in the acquisition buffer
* \param     [OUT] numEvents : Number of events stored in the acquisition buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetNumEvents(int handle, char *buffer, uint32_t buffsize, uint32_t *numEvents);


/**************************************************************************//**
* \brief     Retrieves the information associated with a specified event
* \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
*
* \param     [IN] handle     : digitizer handle
* \param     [IN] buffer     : Address of the acquisition buffer 
* \param     [IN] bufferSize : acquisition buffer size (in samples)
* \param     [IN] numEvents  : Number of events stored in the acquisition buffer
* \param     [OUT] eventInfo : Event Info structure containing the information about the specified event
* \param     [OUT] EventPtr  : Pointer to the requested event in the acquisition buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetEventInfo(int handle, char *buffer, uint32_t buffsize, int32_t numEvent, CAEN_DGTZ_EventInfo_t *eventInfo, char **EventPtr);


/**************************************************************************//**
* \brief     Decodes a specified event stored in the acquisition buffer
* \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
*
* \param     [IN]  handle   : digitizer handle
* \param     [IN]  EventPtr : pointer to the requested event in the acquisition buffer
* \param     [OUT] Evt      : event structure with the requested event data
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DecodeEvent(int handle, char *evtPtr, void **Evt);


/**************************************************************************//**
* \brief     Releases the event returned by the CAEN_DGTZ_DecodeEvent
* \deprecated On DPP-PHA, DPP-PSD and DPP-CI use the <b>new readout API</b>
*
* \param     [IN] handle : digitizer handle
* \param     [IN] Evt    : event structure that store the decoded event.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeEvent(int handle, void **Evt);


/**************************************************************************//**
* \brief     Decodes and returns all the DPP events stored in the acquisition buffers.
* \note     Part of the <b>new readout API</b>
*
* \param    [IN]  handle     : digitizer handle
* \param     [IN]  buffer     : address of the acquisition buffer
* \param     [IN]  buffsize   : acquisition buffer size (in samples)
* \param     [OUT] events     : pointer to the event list (allocated via MallocDPPEvents)
* \param    [OUT] numEventsArray  : pointer to an array of int which will contain the number of events found per channel
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPPEvents(int handle, char *buffer, uint32_t buffsize, void **events, uint32_t *numEventsArray);


/**************************************************************************//**
* \brief     Allocates the event buffer matrix which is handled by CAEN_DGTZ_GetDPPEvents.
*           The matrix has one event array per channel and must be declared as a MAX_CH-sized array of pointers.
* \note     Part of the <b>new readout API</b>
*
* \param     [IN]  handle         : digitizer handle
* \param     [OUT] events         : pointer to the event matrix, which shall be of type:
*                                   - CAEN_DGTZ_DPP_PHA_Event_t, for DPP-PHA,
*                                   - CAEN_DGTZ_DPP_PSD_Event_t, for DPP-PSD
*                                   - CAEN_DGTZ_DPP_CI_Event_t, for DPP-CI
* \param    [OUT] allocatedSize  : size in bytes of the event list
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_MallocDPPEvents(int handle, void **events, uint32_t *allocatedSize);


/**************************************************************************//**
* \brief     Deallocates the event buffer matrix.
* \note     Part of the <b>new readout API</b>
*
* \param     [IN]  handle  : digitizer handle
* \param     [IN] events   : pointer to the event buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeDPPEvents(int handle, void **events);


/**************************************************************************//**
* \brief     Allocates the waveform buffer, which is used by CAEN_DGTZ_DecodeDPPWaveforms.
* \note     Part of the <b>new readout API</b>
*
* \param     [IN]  handle         : digitizer handle
* \param     [OUT] waveforms      : pointer to the waveform buffer, which shall be of type:
*                                  - CAEN_DGTZ_DPP_PHA_Waveforms_t, for DPP-PHA
*                                   - CAEN_DGTZ_DPP_PSD_Waveforms_t, for DPP-PSD
*                                   - CAEN_DGTZ_DPP_CI_Waveforms_t, for DPP-CI
* \param    [OUT] allocatedSize  : size in bytes of the waveform buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_MallocDPPWaveforms(int handle, void **waveforms, uint32_t *allocatedSize);


/**************************************************************************//**
* \brief     Deallocates the waveform buffer.
* \note     Part of the <b>new readout API</b>
*
* \param [IN] handle     : digitizer handle
* \param [IN] waveforms  : pointer to the waveform buffer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_FreeDPPWaveforms(int handle, void *Waveforms);


/**************************************************************************//**
* \brief     Decodes the waveforms contained inside an event.
* \note     Part of the <b>new readout API</b>
*
* \param [IN] handle     : digitizer handle
* \param [IN] events     : pointer to the event
* \param [OUT] waveforms : pointer to the (preallocated) waveform list
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_DecodeDPPWaveforms(int handle, void *event, void *waveforms);


/**************************************************************************//**
* \brief     Sets the number of events that each aggregate will contain
*
* \param [IN] handle      : digitizer handle
* \param [IN] numEvents   : number of events per aggregate
* \param [IN] channel     : optional parameter in the form of an int to specify the channel
                               (required for DPP-PSD and DPP-CI, ignored by DPP-PHA)
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetNumEventsPerAggregate(int handle, uint32_t numEvents, ...);


/**************************************************************************//**
* \brief     Gets the number of events that each aggregate will contain 
* \deprecated 
*
* \param     [IN] handle      : digitizer handle
* \param     [OUT] numEvents  : number of events per aggregate
* \param     [IN] channel     : optional parameter in the form of an int to specify the channel
                               (required for DPP-PSD and DPP-CI, ignored by DPP-PHA)
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetNumEventsPerAggregate(int handle, uint32_t *numEvents, ...);


/**************************************************************************//**
* \brief     Set event aggregation parameters
* \note     Only for DPP-PHA, DPP-PSD and DPP-CI
*
* \param     [IN] handle      : digitizer handle
* \param     [IN] threshold   : Specifies how many events to let accumulate in the board memory before they are rendered available for readout. 
*                              A low number maximizes responsiveness, since data are read as soon as they are stored in memory, while a high number maximizes efficiency, since fewer transfers are made.
*                              Supplying 0 will let the library choose the best value depending on acquisition mode and other parameters.
* \param     [IN] maxsize     : Specifies the maximum size in bytes of the event buffer on the PC side.
*                              This parameter might be useful in case the computer has very low RAM. 
*                              Normally, though, it is safe to supply 0 as this parameter, so that the library will choose an appropriate value automatically.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPEventAggregation(int handle, int threshold, int maxsize);


/**************************************************************************//**
* \brief     Set DPP configuration parameters for DPP-PHA, DPP-PSD or DPP-CI
* \note     Only for DPP-PHA, DPP-PSD and DPP-CI
*
* \param     [IN] handle      : digitizer handle
* \param     [IN] channelMask : A bit mask indicating the channels to apply the DPP parameters  (not used with x743 family)
* \param     [IN] params      : Pointer to a preallocated struct of type:
*                               - CAEN_DGTZ_DPP_PHA_Params_t, in case of DPP-PHA
*                               - CAEN_DGTZ_DPP_PSD_Params_t,  in case of DPP-PSD
*                               - CAEN_DGTZ_DPP_CI_Params_t, in case of DPP-CI
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPParameters(int handle, uint32_t channelMask, void* params);


/**************************************************************************//**
* \brief     Set the DPP acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode: The DPP acquisition mode
* \param     [IN] param: The acquisition data to retrieve in acquisition
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPAcquisitionMode(int handle,CAEN_DGTZ_DPP_AcqMode_t mode, CAEN_DGTZ_DPP_SaveParam_t param);


/**************************************************************************//**
* \brief     Get the information about the DPP acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [OUT] mode: The DPP acquisition mode
* \param     [OUT] param: The acquisition data to retrieve in acquisition
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPPAcquisitionMode(int handle,CAEN_DGTZ_DPP_AcqMode_t *mode, CAEN_DGTZ_DPP_SaveParam_t *param);


/**************************************************************************//**
* \brief     Sets the DPP Trigger mode (DPP-PSD and DPP-CI only)
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode:   the desired trigger mode:
*                         - CAEN_DGTZ_DPP_TriggerMode_Normal
*                          - CAEN_DGTZ_DPP_TriggerMode_Coincidence
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPPTriggerMode(int handle, CAEN_DGTZ_DPP_TriggerMode_t mode);


/**************************************************************************//**
* \brief     Gets the current DPP Trigger mode (DPP-PSD and DPP-CI only)
*
* \param     [IN] handle : digitizer handle
* \param     [OUT] mode:   the current trigger mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPPTriggerMode(int handle, CAEN_DGTZ_DPP_TriggerMode_t *mode);


/**************************************************************************//**
* \brief     Set the information about the output signal of the DPP-PHA acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode: The Virtual Probe mode
* \param     [IN] vp1: The Virtual Probe1 mode
* \param     [IN] vp2: The Virtual Probe2 mode
* \param     [IN] dp: The Digital Probe mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
DPP_DEPRECATED(CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPP_PHA_VirtualProbe(int handle,CAEN_DGTZ_DPP_VirtualProbe_t mode, CAEN_DGTZ_DPP_PHA_VirtualProbe1_t vp1, CAEN_DGTZ_DPP_PHA_VirtualProbe2_t vp2, CAEN_DGTZ_DPP_PHA_DigitalProbe_t dp));


/**************************************************************************//**
* \brief     Get the information about the output signal of the DPP-PHA acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [OUT] mode: The Virtual Probe mode
* \param     [OUT] vp1: The Virtual Probe1 mode
* \param     [OUT] vp2: The Virtual Probe2 mode
* \param     [OUT] dp: The Digital Probe mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
DPP_DEPRECATED(CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_PHA_VirtualProbe(int handle,CAEN_DGTZ_DPP_VirtualProbe_t *mode, CAEN_DGTZ_DPP_PHA_VirtualProbe1_t *vp1, CAEN_DGTZ_DPP_PHA_VirtualProbe2_t *vp2, CAEN_DGTZ_DPP_PHA_DigitalProbe_t *dp));


/**************************************************************************//**
* \brief     Set the information about the output signal of the DPP-PSD acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode: The Virtual Probe mode
* \param     [IN] vp: The Virtual Probe mode     NOTE: ignored for x751, VirtualProbes are always Input and Baseline
* \param     [IN] dp1: The Digital Probe1 mode
* \param     [IN] dp2: The Digital Probe2 mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
DPP_DEPRECATED(CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPP_PSD_VirtualProbe(int handle, CAEN_DGTZ_DPP_VirtualProbe_t mode, CAEN_DGTZ_DPP_PSD_VirtualProbe_t vp, CAEN_DGTZ_DPP_PSD_DigitalProbe1_t dp1, CAEN_DGTZ_DPP_PSD_DigitalProbe2_t dp2));


/**************************************************************************//**
* \brief     Get the information about the output signal of the DPP-PSD acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [OUT] mode: The Virtual Probe mode
* \param     [OUT] vp:  The Virtual Probe mode
* \param     [OUT] dp1: The Digital Probe1 mode
* \param     [OUT] dp2: The Digital Probe2 mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
DPP_DEPRECATED(CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_PSD_VirtualProbe(int handle, CAEN_DGTZ_DPP_VirtualProbe_t *mode, CAEN_DGTZ_DPP_PSD_VirtualProbe_t *vp, CAEN_DGTZ_DPP_PSD_DigitalProbe1_t *dp1, CAEN_DGTZ_DPP_PSD_DigitalProbe2_t *dp2));


/**************************************************************************//**
* \brief     Set the information about the output signal of the DPP-CI acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [IN] mode: The Virtual Probe mode
* \param     [IN] vp: The Virtual Probe mode
* \param     [IN] dp1: The Digital Probe1 mode
* \param     [IN] dp2: The Digital Probe2 mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
DPP_DEPRECATED(CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPP_CI_VirtualProbe(int handle, CAEN_DGTZ_DPP_VirtualProbe_t mode, CAEN_DGTZ_DPP_CI_VirtualProbe_t vp, CAEN_DGTZ_DPP_CI_DigitalProbe1_t dp1, CAEN_DGTZ_DPP_CI_DigitalProbe2_t dp2));


/**************************************************************************//**
* \brief     Get the information about the output signal of the DPP-CI acquisition mode.
*
* \param     [IN] handle : digitizer handle
* \param     [OUT] mode: The Virtual Probe mode
* \param     [OUT] vp:  The Virtual Probe mode
* \param     [OUT] dp1: The Digital Probe1 mode
* \param     [OUT] dp2: The Digital Probe2 mode
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
DPP_DEPRECATED(CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_CI_VirtualProbe(int handle, CAEN_DGTZ_DPP_VirtualProbe_t *mode, CAEN_DGTZ_DPP_CI_VirtualProbe_t *vp, CAEN_DGTZ_DPP_CI_DigitalProbe1_t *dp1, CAEN_DGTZ_DPP_CI_DigitalProbe2_t *dp2));


/*****************************************************************************
* \brief     Set the virtual probe to be displayed on the given trace
*
* \param     [IN] handle : digitizer handle
* \param     [IN] trace: The Trace to be affected
* \param     [IN] probe:  The Virtual Probe to be set on the given trace
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetDPP_VirtualProbe(int handle, int trace, int probe);


/*****************************************************************************
* \brief     Get the virtual probe currently displayed on the given trace
*
* \param     [IN] handle : digitizer handle
* \param     [IN] trace: The Trace to be get the probe of
* \param     [OUT] probe: The Virtual Probe displayed on the given trace
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_VirtualProbe(int handle, int trace, int *probe);

/*****************************************************************************
* \brief     Get the list of virtual probes supported on board's given trace
*
* \param     [IN] handle : digitizer handle
* \param     [IN] trace: The Trace to be get the probes list of
* \param     [OUT] probes: The list of Virtual Probes supported by the trace
*                          It Must be an array of length MAX_SUPPORTED_PROBES
* \param     [OUT] numProbes: The number of Probes supported by the trace.
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_SupportedVirtualProbes(int handle, int trace, int probes[], int *numProbes);


/*****************************************************************************
* \brief     Get the name of the given virtual probe
*
* \param     [IN] probe: The Virtual Probe to get the name of
* \param     [OUT] name: The name of the given probe
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetDPP_VirtualProbeName(int probe, char name[]);


/**************************************************************************//**
* \brief     Allocate the memory for the event
*
* \param     [IN] handle : digitizer handle
* \param     [IN] Evt: the reference to the Event pointer 
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_AllocateEvent(int handle, void **Evt);


/**************************************************************************//**
* \brief     Sets the IO Level
*
* \param     [IN] handle  : digitizer handle
* \param     [IN] level   : The level to set
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetIOLevel(int handle, CAEN_DGTZ_IOLevel_t level);


/**************************************************************************//**
* \brief     Gets the IO Level
*
* \param     [IN] handle  : digitizer handle
* \param     [OUT] level  : The IO level of the digitizer
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetIOLevel(int handle, CAEN_DGTZ_IOLevel_t *level);


/**************************************************************************//**
* \brief     Sets the trigger polarity of a specified channel
*
* \param     [IN] handle  : digitizer handle
* \param    [IN] channel : select channel to set the trigger polarity
* \param     [IN] Polarity   : The polarity to set
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_SetTriggerPolarity(int handle, uint32_t channel, CAEN_DGTZ_TriggerPolarity_t Polarity);


/**************************************************************************//**
* \brief     Gets the trigger polarity of a specified channel
*
* \param     [IN] handle  : digitizer handle
* \param    [IN] channel : select channel to get the trigger polarity
* \param     [OUT] Polarity   : The polarity of the specified channel
* \return  0 = Success; negative numbers are error codes
******************************************************************************/
CAEN_DGTZ_ErrorCode CAENDGTZ_API CAEN_DGTZ_GetTriggerPolarity(int handle, uint32_t channel, CAEN_DGTZ_TriggerPolarity_t *Polarity);













// 
// note.cc ends here
