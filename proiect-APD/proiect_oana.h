/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ACQ_PANEL                        1       /* callback function: OnAcqPanel */
#define  ACQ_PANEL_BINARYSWITCH_2         2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  ACQ_PANEL_LOAD_3                 3       /* control type: command, callback function: OnSave2 */
#define  ACQ_PANEL_FREQ_PEAK_2            4       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_POWER_PEAK_2           5       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_FREQ_PEAK              6       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_POWER_PEAK             7       /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_WAVE_FERESTRUIT        8       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_SPECTR_FILT_FER  9       /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_SPECTRU          10      /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_WAVE_FILTRED     11      /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_GRAPH_WINDOW           12      /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_WAVE_DATA              13      /* control type: graph, callback function: (none) */
#define  ACQ_PANEL_SECUNDE                14      /* control type: ring, callback function: schimbareSec */
#define  ACQ_PANEL_NUMAR_PUNCTE           15      /* control type: ring, callback function: (none) */
#define  ACQ_PANEL_FILTER_TYPE            16      /* control type: ring, callback function: SetFilterType */
#define  ACQ_PANEL_TIP_AFISARE            17      /* control type: ring, callback function: SelectareTipAfisare */
#define  ACQ_PANEL_IDC_CUT_FREQ           18      /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_IDC_ORDER              19      /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_IDC_FPASS              20      /* control type: numeric, callback function: (none) */
#define  ACQ_PANEL_WINDOW_TYPE            21      /* control type: ring, callback function: SetWindowType */
#define  ACQ_PANEL_TIMER                  22      /* control type: timer, callback function: OnTimer */

#define  MAIN_PANEL                       2       /* callback function: OnMainPanel */
#define  MAIN_PANEL_LOAD_4                2       /* control type: command, callback function: OnApplyDer */
#define  MAIN_PANEL_LOAD_3                3       /* control type: command, callback function: OnSave */
#define  MAIN_PANEL_LOAD_2                4       /* control type: command, callback function: OnPlotEnvelope */
#define  MAIN_PANEL_LOAD                  5       /* control type: command, callback function: OnLoadButtonCB */
#define  MAIN_PANEL_APLICA                6       /* control type: command, callback function: OnApl */
#define  MAIN_PANEL_COMMANDBUTTON_2       7       /* control type: command, callback function: OnNext */
#define  MAIN_PANEL_COMMANDBUTTON         8       /* control type: command, callback function: OnPrev */
#define  MAIN_PANEL_RING_2                9       /* control type: ring, callback function: DimFereastra */
#define  MAIN_PANEL_RING                  10      /* control type: ring, callback function: OnRing */
#define  MAIN_PANEL_TEXTMSG               11      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_FILTERED_DATA         12      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH_RAW_DATA        13      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_TRECERI_PRIN_0        14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIANA               15      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DISPERSIE             16      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIE                 17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_INDEX_MAX             18      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MAX                   19      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_STOP                  20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_START                 21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_INDEX_MIN             22      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_HISTOGRAMA            23      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_MIN                   24      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_BINARYSWITCH          25      /* control type: binary, callback function: OnSwitch */
#define  MAIN_PANEL_START_3               26      /* control type: string, callback function: (none) */
#define  MAIN_PANEL_BINARYSWITCH_2        27      /* control type: binary, callback function: OnSwitchPanelCB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DimFereastra(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAcqPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApplyDer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPlotEnvelope(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnRing(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSave2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK schimbareSec(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SelectareTipAfisare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetFilterType(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetWindowType(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif