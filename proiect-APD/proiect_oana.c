#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "SwitchPanels.h"
#include "proiect_oana.h"
#include "toolbox.h"

#define SAMPLE_RATE		0
#define NPOINTS			1
static int mainPanel;

double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
int startVal=0;
int stopVal=6;
double *vectForSeconds=0;
int dim;
int dimensiuneFereastra=32;
int anvelopa=0;
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
int binarySwitch=0;
double *envelope=0;
int acqPanel = 0;
int nrPuncte;
double *waveDataPerNrPuncte=0;
int tipAfisare=0;
int nrFereasra=0;
int secunda;
int TipFiltru=0;
int TipFereastra=0;
double*window;
double *windowWave;
double*output;
int numarFerestre;
int afisare=0;
int main (int argc, char *argv[])
{
Error:
	/* clean up */
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "proiect_oana.uir", MAIN_PANEL)) < 0)
		return -1;
	acqPanel = LoadPanel (0,  "proiect_oana.uir", ACQ_PANEL);
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	free(envelope);
	free(vectForSeconds);
	free(waveData);
	return 0;
}
void AfisarePeInterfata(int panel,double*vect,int dim)
{
	double maxVal=0.0;
	double minVal=0.0;
	int maxIndex=0;
	int minIndex=0;
	double meanVal=0.0;
	double mediana=0.0;
	double dispersie=0.0;
	double vectAux1[100];
	int vectAux2[100];
	int intervals=15;
	MaxMin1D( vect,dim,&maxVal,&maxIndex,&minVal,&minIndex);
	Mean( vect,dim,&meanVal);
	Median( vect,dim, &mediana);
	StdDev ( vect,dim, &meanVal, &dispersie);
	Histogram( vect,dim,minVal-1,maxVal+1,vectAux2,vectAux1,intervals);
	DeleteGraphPlot (panel,MAIN_PANEL_HISTOGRAMA, -1, VAL_DELAYED_DRAW);
	PlotXY (panel,MAIN_PANEL_HISTOGRAMA,(void*)vectAux1,  (void*)vectAux2, intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
				
				
	SetCtrlVal(panel,MAIN_PANEL_MIN,minVal);
	SetCtrlVal(panel,MAIN_PANEL_MAX,maxVal);
	SetCtrlVal(panel,MAIN_PANEL_MEDIE,meanVal);
	SetCtrlVal(panel,MAIN_PANEL_INDEX_MIN,minIndex);
	SetCtrlVal(panel,MAIN_PANEL_INDEX_MAX,maxIndex);
	SetCtrlVal(panel,MAIN_PANEL_MEDIANA,mediana);
	SetCtrlVal(panel,MAIN_PANEL_DISPERSIE,dispersie);
	int treceri=0,i;
	for(i=0;i<dim-1;i++)
	{
		if(vect[i]==0 ||(vect[i]>0 && vect[i+1]<0) || (vect[i]<0 &&vect[i+1]>0))
		{
			treceri++;
		}
	}
	SetCtrlVal(panel,MAIN_PANEL_TRECERI_PRIN_0,treceri);
}
void FitrarePrinMediere(double*vect,int dim,double*y)
{
	int i;
	int sum=0;
	for(i=0;i<dim;i++)
	{
		if(i<dimensiuneFereastra)
		{
			if(i<dimensiuneFereastra-1)
			{
				y[i]=vect[i];
				sum+=y[i];
				
			}
			else
			{
				sum+=y[i];
				y[i]=(double)sum/(double)dimensiuneFereastra;
			}
			
		}
		else
		{
			sum=sum-vect[i-dimensiuneFereastra]+vect[i];
			y[i]=(double)sum/(double)dimensiuneFereastra;
		}
	}
	
}
int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	
		case EVENT_COMMIT:
			DeleteGraphPlot (panel,MAIN_PANEL_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (panel,MAIN_PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			GetCtrlVal(panel,MAIN_PANEL_BINARYSWITCH,&binarySwitch);
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			//alocare memorie pentru numarul de puncte
			waveData = (double *)calloc(npoints, sizeof(double));
				
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				
			if(binarySwitch==0)
			{	
				PlotY(panel, MAIN_PANEL_GRAPH_RAW_DATA,waveData,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				AfisarePeInterfata(panel,waveData,npoints);
				break;
			}
	}
	return 0;
}

int CVICALLBACK OnMainPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
		
			break;
	}
	return 0;
}
void afisarePerSecond(int panel)
{
		dim=waveInfo[NPOINTS]/6;
		vectForSeconds = (double *) calloc(dim, sizeof(double));
		int i;
		for(i=0;i<dim;i++)
		{
			vectForSeconds[i]=waveData[startVal*dim+i];
		}
		//afisare pe grapf
		DeleteGraphPlot (panel,MAIN_PANEL_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
		DeleteGraphPlot (panel,MAIN_PANEL_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
		PlotY(panel, MAIN_PANEL_GRAPH_RAW_DATA,vectForSeconds,dim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
		AfisarePeInterfata(panel,vectForSeconds,dim);
}
int CVICALLBACK OnNext (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(stopVal<6)
			{
				startVal++;
				stopVal++;
				SetCtrlVal(panel,MAIN_PANEL_START,startVal);
				SetCtrlVal(panel,MAIN_PANEL_STOP,stopVal);
				
				afisarePerSecond(panel);
			}
			else
			{
				MessagePopup("Eroare","Nu se poate executa aceasta operatiune");
			}
			
			break;
	}
	return 0;
}
int CVICALLBACK OnPrev (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		
			if(stopVal>1)
			{
				startVal--;
				stopVal--;
				SetCtrlVal(panel,MAIN_PANEL_START,startVal);
				SetCtrlVal(panel,MAIN_PANEL_STOP,stopVal);
				
				afisarePerSecond(panel);
			}
			else
			{
				MessagePopup("Eroare","Nu se poate executa aceasta operatiune");
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitch (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,MAIN_PANEL_BINARYSWITCH,&binarySwitch);
			if(binarySwitch==1)
			{
				SetCtrlAttribute(panel,MAIN_PANEL_START,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,MAIN_PANEL_STOP,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,MAIN_PANEL_COMMANDBUTTON_2,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,MAIN_PANEL_COMMANDBUTTON,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,MAIN_PANEL_LOAD,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,MAIN_PANEL_START,ATTR_CTRL_VAL,0);
				SetCtrlAttribute(panel,MAIN_PANEL_STOP,ATTR_CTRL_VAL,1);
				GetCtrlAttribute(panel,MAIN_PANEL_START,ATTR_CTRL_VAL,&startVal);
				GetCtrlAttribute(panel,MAIN_PANEL_STOP,ATTR_CTRL_VAL,&stopVal);
				afisarePerSecond(panel);
			}
			else{
				SetCtrlAttribute(panel,MAIN_PANEL_START,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,MAIN_PANEL_STOP,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,MAIN_PANEL_COMMANDBUTTON_2,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,MAIN_PANEL_COMMANDBUTTON,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,MAIN_PANEL_LOAD,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,MAIN_PANEL_START,ATTR_CTRL_VAL,0);
				SetCtrlAttribute(panel,MAIN_PANEL_STOP,ATTR_CTRL_VAL,6);
				GetCtrlAttribute(panel,MAIN_PANEL_START,ATTR_CTRL_VAL,&startVal);
				GetCtrlAttribute(panel,MAIN_PANEL_STOP,ATTR_CTRL_VAL,&stopVal);
				SetCtrlAttribute(panel,MAIN_PANEL_LOAD,ATTR_LABEL_TEXT,"Load wave file");
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnApl (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int filtru;
	double *y;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(panel,MAIN_PANEL_RING,ATTR_CTRL_INDEX,&filtru);
			if(filtru==0)
			{
				//mediere
				if(binarySwitch==0)
				{
					DeleteGraphPlot (panel,MAIN_PANEL_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
					y= (double *)calloc(npoints, sizeof(double));
					FitrarePrinMediere(waveData,npoints,y);
					PlotY(panel, MAIN_PANEL_FILTERED_DATA, y,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_YELLOW);
					AfisarePeInterfata(panel,y,npoints);
					free(y);
				}
				else
				{
					DeleteGraphPlot (panel,MAIN_PANEL_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
					y= (double *)calloc(dim, sizeof(double));
					FitrarePrinMediere(vectForSeconds,dim,y);
					PlotY(panel, MAIN_PANEL_FILTERED_DATA,y,dim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_YELLOW);
					AfisarePeInterfata(panel,y,dim);
					free(y);
				}
			}
			else
			{
				if(binarySwitch==0)
				{
					//element de gradul I
					DeleteGraphPlot (panel,MAIN_PANEL_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
					char alpha[20];
					GetCtrlVal(panel,MAIN_PANEL_START_3,alpha);
					double a=strtod(alpha,NULL);
					double *filtru=0;
					filtru = (double *) calloc(npoints, sizeof(double));
					filtru[0]=waveData[0];
					int i;
					for(i=1;i<npoints;i++)
					{
						filtru[i]=(1-a)*filtru[i-1]+a*waveData[i];
					}
					PlotY(panel,MAIN_PANEL_FILTERED_DATA, filtru,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_YELLOW);
					AfisarePeInterfata(panel,filtru,npoints);
					free(filtru);
				}
				else
				{
					DeleteGraphPlot (panel,MAIN_PANEL_FILTERED_DATA, -1, VAL_IMMEDIATE_DRAW);
					char alpha[20];
					GetCtrlVal(panel,MAIN_PANEL_START_3,alpha);
					double a=strtod(alpha,NULL);
					double *filtru=0;
					filtru = (double *) calloc(dim, sizeof(double));
					filtru[0]=vectForSeconds[0];
					int i;
					for(i=1;i<dim;i++)
					{
						filtru[i]=(1-a)*filtru[i-1]+a*vectForSeconds[i];
					}
					PlotY(panel,MAIN_PANEL_FILTERED_DATA, filtru,dim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_YELLOW);
					AfisarePeInterfata(panel,filtru,dim);
					free(filtru);
				}
			}
			break;
	}
	return 0;
}
int CVICALLBACK OnRing (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int filtru;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(panel,MAIN_PANEL_RING,ATTR_CTRL_INDEX,&filtru);

			if(filtru==0)
			{
				//mediere
				SetCtrlAttribute(panel,MAIN_PANEL_START_3,ATTR_DIMMED,1);
				SetCtrlAttribute(panel,MAIN_PANEL_RING_2,ATTR_DIMMED,0);
			}
			else
			{
				//element de gradul I
				SetCtrlAttribute(panel,MAIN_PANEL_START_3,ATTR_DIMMED,0);
				SetCtrlAttribute(panel,MAIN_PANEL_RING_2,ATTR_DIMMED,1);
			}
			break;
	}
	return 0;
}





int CVICALLBACK OnPlotEnvelope (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(anvelopa==0)
			{
				//LaunchExecutable("python envelope.py");
				//Delay(0);
				envelope=(double*)calloc(npoints,sizeof(double));
				FileToArray("envelope.txt", envelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			}
			if(binarySwitch==0)
			{
				anvelopa=1;
				PlotY(panel, MAIN_PANEL_GRAPH_RAW_DATA, envelope,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			}
			else
			{
				double*derivataPerSecond=0;
				derivataPerSecond=(double*)calloc(dim,sizeof(double));
				int i;
				for(i=0;i<dim;i++)
				{
					derivataPerSecond[i]=envelope[startVal*dim+i];
				}
				PlotY(panel, MAIN_PANEL_GRAPH_RAW_DATA,derivataPerSecond,dim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			}
			
			
			break;
	}
	return 0;
}

int CVICALLBACK OnSave (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int bitmapId=0;
			GetCtrlDisplayBitmap(panel,MAIN_PANEL_GRAPH_RAW_DATA,1,&bitmapId);
			char*filename=0;
			filename=(char*)calloc(100,sizeof(char));
			sprintf(filename, "raw_data_%d_%d.png", startVal,stopVal);
			SaveBitmapToPNGFile(bitmapId,filename);
			DiscardBitmap(bitmapId);
			GetCtrlDisplayBitmap(panel,MAIN_PANEL_FILTERED_DATA,1,&bitmapId);
			sprintf(filename, "filtred_data_%d_%d.png", startVal,stopVal);
			SaveBitmapToPNGFile(bitmapId,filename);
			DiscardBitmap(bitmapId);
			free(filename);
			break;
	}
	return 0;
}

//difference

int CVICALLBACK OnApplyDer (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double *derivate=0;
			if(binarySwitch==0)
			{
				derivate=(double*)calloc(npoints,sizeof(double));
				Difference(waveData,npoints,1.0,0.0,0.0,derivate);
				PlotY(panel, MAIN_PANEL_FILTERED_DATA, derivate,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
				
			}
			else
			{
				derivate=(double*)calloc(dim,sizeof(double));
				Difference(vectForSeconds,dim,1.0,0.0,0.0,derivate);
				PlotY(panel, MAIN_PANEL_FILTERED_DATA,derivate,dim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}
			free(derivate);
			break;
	}
	return 0;
}

int CVICALLBACK DimFereastra (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int fereastra=0;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(panel,MAIN_PANEL_RING_2,ATTR_CTRL_INDEX,&fereastra);
			switch(fereastra)
			{
				case 0:
					dimensiuneFereastra=32;
					break;
				case 1:
					dimensiuneFereastra=16;
					break;
				case 2:
					dimensiuneFereastra=8;
					break;
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel)
			{
				SetCtrlVal(acqPanel, ACQ_PANEL_BINARYSWITCH_2, 1);
				DisplayPanel(acqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_BINARYSWITCH_2, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnAcqPanel (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}
int aplicaSpetruPerTotal()
{
	if(tipAfisare==0)
	{
			WindowConst winConst;
				double *convertedSpectrum;//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
				double powerPeak = 0.0;//valoarea maxima din spectru de putere (din autoSpectrum)
				double freqPeak = 0.0;//variabila ce reprezinta pasul in domeniul frecventei
				double df = 0.0;//voltage signal - descriere a semnalului achizitionat
				char unit[32] = "V";//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
				double *autoSpectrum;
				convertedSpectrum=(double*)calloc(nrPuncte/2,sizeof(double));
				autoSpectrum=(double*)calloc(nrPuncte/2,sizeof(double));
				ScaledWindowEx(windowWave,nrPuncte,RECTANGLE_,0,&winConst);
				AutoPowerSpectrum(windowWave,nrPuncte,1.0/sampleRate,autoSpectrum,&df);
				//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului 
				PowerFrequencyEstimate(autoSpectrum,nrPuncte/2,-1.0,winConst,df,7,&freqPeak,&powerPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_FREQ_PEAK_2, freqPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_POWER_PEAK_2, powerPeak);
		    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
				//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			 	SpectrumUnitConversion(autoSpectrum, nrPuncte/2,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
				
				//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
				DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_SPECTR_FILT_FER, -1, VAL_IMMEDIATE_DRAW);	
				//plotez spectrul semnalului
		    	PlotWaveform( acqPanel,ACQ_PANEL_GRAPH_SPECTR_FILT_FER, convertedSpectrum, nrPuncte/2,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
		                             VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			
	}
	if(tipAfisare==1)
	{
				WindowConst winConst;
				double *convertedSpectrum;//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
				double powerPeak = 0.0;//valoarea maxima din spectru de putere (din autoSpectrum)
				double freqPeak = 0.0;//variabila ce reprezinta pasul in domeniul frecventei
				double df = 0.0;//voltage signal - descriere a semnalului achizitionat
				char unit[32] = "V";//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
				double *autoSpectrum;
				convertedSpectrum=(double*)calloc(dim/2,sizeof(double));
				autoSpectrum=(double*)calloc(dim/2,sizeof(double));
				ScaledWindowEx(windowWave,dim,RECTANGLE_,0,&winConst);
				AutoPowerSpectrum(windowWave,dim,1.0/sampleRate,autoSpectrum,&df);
				//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului 
				PowerFrequencyEstimate(autoSpectrum,dim/2,-1.0,winConst,df,7,&freqPeak,&powerPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_FREQ_PEAK_2, freqPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_POWER_PEAK_2, powerPeak);
		    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
				//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			 	SpectrumUnitConversion(autoSpectrum,dim/2,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
				
				//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
				DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_SPECTR_FILT_FER, -1, VAL_IMMEDIATE_DRAW);	
				//plotez spectrul semnalului
		    	PlotWaveform( acqPanel,ACQ_PANEL_GRAPH_SPECTR_FILT_FER, convertedSpectrum, dim/2,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
		                             VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			
	}
	if(tipAfisare==2)
	{
				WindowConst winConst;
				double *convertedSpectrum;//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
				double powerPeak = 0.0;//valoarea maxima din spectru de putere (din autoSpectrum)
				double freqPeak = 0.0;//variabila ce reprezinta pasul in domeniul frecventei
				double df = 0.0;//voltage signal - descriere a semnalului achizitionat
				char unit[32] = "V";//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
				double *autoSpectrum;
				convertedSpectrum=(double*)calloc(npoints/2,sizeof(double));
				autoSpectrum=(double*)calloc(npoints/2,sizeof(double));
				ScaledWindowEx(windowWave,npoints,RECTANGLE_,0,&winConst);
				AutoPowerSpectrum(windowWave,npoints,1.0/sampleRate,autoSpectrum,&df);
				//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului 
				PowerFrequencyEstimate(autoSpectrum,npoints/2,-1.0,winConst,df,7,&freqPeak,&powerPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_FREQ_PEAK_2, freqPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_POWER_PEAK_2, powerPeak);
		    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
				//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			 	SpectrumUnitConversion(autoSpectrum, npoints/2,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
				
				//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
				DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_SPECTR_FILT_FER, -1, VAL_IMMEDIATE_DRAW);	
				//plotez spectrul semnalului
		    	PlotWaveform( acqPanel,ACQ_PANEL_GRAPH_SPECTR_FILT_FER, convertedSpectrum, npoints/2,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
		                             VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			
	}
	return 0;
}

int afisareFereastra()
{
	
	if(tipAfisare==0)
	{
		window = (double*)malloc(nrPuncte * sizeof(double));
		for (int i = 0; i < nrPuncte; i++)
		{
			window[i]=1.0;
		}
		if(TipFereastra==0)//hamming
		{
			HamWin(window, nrPuncte);
			
		}
		if(TipFereastra==1)//Blackman-Harris
		{
			HanWin(window, nrPuncte);
		}
		
		DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);	
		PlotY(acqPanel,ACQ_PANEL_GRAPH_WINDOW,window,nrPuncte,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);	
		windowWave=(double*)calloc(nrPuncte,sizeof(double));
		
		Mul1D(output+nrFereasra*nrPuncte,window,nrPuncte,windowWave);
		DeleteGraphPlot(acqPanel,ACQ_PANEL_WAVE_FERESTRUIT,-1, VAL_IMMEDIATE_DRAW);
		PlotY(acqPanel,ACQ_PANEL_WAVE_FERESTRUIT,windowWave,nrPuncte,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);		
	
	}
	if(tipAfisare==1)
	{
			window = (double*)malloc(dim * sizeof(double));
		for (int i = 0; i < dim; i++)
		{
			window[i]=1.0;
		}
		if(TipFereastra==0)//hamming
		{
			
			HamWin(window, dim);
			
		}
		if(TipFereastra==1)//Blackman-Harris
		{
			HanWin(window, dim);
		}
		
		DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);	
		PlotY(acqPanel,ACQ_PANEL_GRAPH_WINDOW,window,dim,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);	
		windowWave=(double*)calloc(dim,sizeof(double));
		
		Mul1D(output,window,dim,windowWave);
		DeleteGraphPlot(acqPanel,ACQ_PANEL_WAVE_FERESTRUIT,-1, VAL_IMMEDIATE_DRAW);
		PlotY(acqPanel,ACQ_PANEL_WAVE_FERESTRUIT,windowWave,dim,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);		
	}
	if(tipAfisare==2)
	{
			window = (double*)malloc(npoints * sizeof(double));
		for (int i = 0; i < npoints; i++)
		{
			window[i]=1.0;
		}
		if(TipFereastra==0)//hamming
		{
			
			HamWin(window, npoints);
			
		}
		if(TipFereastra==1)//Blackman-Harris
		{
			HanWin(window, npoints);
		}
		
		DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);	
		PlotY(acqPanel,ACQ_PANEL_GRAPH_WINDOW,window,npoints,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);	
		windowWave=(double*)calloc(npoints,sizeof(double));
		
		Mul1D(output,window,npoints,windowWave);
		DeleteGraphPlot(acqPanel,ACQ_PANEL_WAVE_FERESTRUIT,-1, VAL_IMMEDIATE_DRAW);
		PlotY(acqPanel,ACQ_PANEL_WAVE_FERESTRUIT,windowWave,npoints,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);		

	}
	aplicaSpetruPerTotal();
	return 0;
	
} 
int aplicaFiltru()
{
	int fcut;
	int order;
	int fpass;
	double coef[55];
	GetCtrlVal(acqPanel,ACQ_PANEL_IDC_CUT_FREQ,&fcut);
	GetCtrlVal(acqPanel,ACQ_PANEL_IDC_ORDER,&order);
	GetCtrlVal(acqPanel,ACQ_PANEL_IDC_FPASS,&fpass);
	if(tipAfisare==0)
	{
		
		if(TipFiltru==0)
			{
				output=(double*)calloc(numarFerestre*nrPuncte+55,sizeof(double));
				Ksr_HPF (sampleRate, fcut, 55, coef,4.5);
				Convolve(waveDataPerNrPuncte,numarFerestre*nrPuncte,coef,55,output);
				DeleteGraphPlot(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,-1,VAL_IMMEDIATE_DRAW);
				PlotY(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,output+nrFereasra*nrPuncte,nrPuncte,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
			}
			if(TipFiltru==1)
			{
				output=(double*)calloc(numarFerestre*nrPuncte,sizeof(double));
				InvCh_HPF (waveDataPerNrPuncte,numarFerestre*nrPuncte,numarFerestre*nrPuncte,fcut,40,order,output);
				DeleteGraphPlot(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,-1,VAL_IMMEDIATE_DRAW);
				PlotY(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,output+nrFereasra*nrPuncte,nrPuncte,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
			
			}
			afisareFereastra();

	}
	if(tipAfisare==1)
	{
		
		if(TipFiltru==0)
				{
					output=(double*)calloc(dim+55,sizeof(double));
					Ksr_HPF (sampleRate, fcut, 55, coef,4.5);
					Convolve(vectForSeconds,dim,coef,55,output);
					DeleteGraphPlot(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,-1,VAL_IMMEDIATE_DRAW);
					PlotY(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,output,dim,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
				}
				if(TipFiltru==1)
				{
					output=(double*)calloc(dim,sizeof(double));
					InvCh_HPF (vectForSeconds,dim,dim,fcut,40,order,output);
					DeleteGraphPlot(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,-1,VAL_IMMEDIATE_DRAW);
					PlotY(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,output,dim,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
				
				}
				afisareFereastra();

	}
	if(tipAfisare==2)
	{
		
				if(TipFiltru==0)
				{
					output=(double*)calloc(npoints+55,sizeof(double));
					Ksr_HPF (sampleRate, fcut, 55, coef,4.5);
					Convolve(waveData,npoints,coef,55,output);
					DeleteGraphPlot(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,-1,VAL_IMMEDIATE_DRAW);
					PlotY(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,output,npoints,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
				}
				if(TipFiltru==1)
				{
					output=(double*)calloc(npoints,sizeof(double));
					InvCh_HPF (waveData,npoints,npoints,fcut,40,order,output);
					DeleteGraphPlot(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,-1,VAL_IMMEDIATE_DRAW);
					PlotY(acqPanel,ACQ_PANEL_GRAPH_WAVE_FILTRED,output,npoints,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
				
				}
				afisareFereastra();

	}
	
	return 0;
}

int CVICALLBACK OnTimer (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			GetCtrlVal(acqPanel,ACQ_PANEL_NUMAR_PUNCTE,&nrPuncte);
			WindowConst winConst;
			double *convertedSpectrum;//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
			double powerPeak = 0.0;//valoarea maxima din spectru de putere (din autoSpectrum)
			double freqPeak = 0.0;//variabila ce reprezinta pasul in domeniul frecventei
			double df = 0.0;//voltage signal - descriere a semnalului achizitionat
			char unit[32] = "V";//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
			double *autoSpectrum;
			numarFerestre = (int)(((double)npoints) / nrPuncte);
			if (((double)npoints) / nrPuncte - numarFerestre > 0.0)
			{
				numarFerestre++;
			}
			waveDataPerNrPuncte=(double*)calloc(nrPuncte*numarFerestre,sizeof(double));
			convertedSpectrum=(double*)calloc(nrPuncte/2,sizeof(double));
			autoSpectrum=(double*)calloc(nrPuncte/2,sizeof(double));
			for(int i=0;i<nrPuncte*numarFerestre;i++)//formam wave data astfel incat sa avem un nr complet de puncte si pentru ultima fereastra(completam ultima fereastra cu 0)
			{
				if(i<npoints)
				{
					waveDataPerNrPuncte[i]=waveData[i];
				}
				else
				{
					waveDataPerNrPuncte[i]=0;
				}
			}
			//wave data
			DeleteGraphPlot(acqPanel,ACQ_PANEL_WAVE_DATA,-1, VAL_IMMEDIATE_DRAW);
			PlotY(acqPanel,ACQ_PANEL_WAVE_DATA,waveDataPerNrPuncte+nrFereasra*nrPuncte,nrPuncte,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
			
			ScaledWindowEx(waveDataPerNrPuncte+nrFereasra*nrPuncte,nrPuncte,RECTANGLE_,0,&winConst);
			AutoPowerSpectrum(waveDataPerNrPuncte+nrFereasra*nrPuncte,nrPuncte,1.0/sampleRate,autoSpectrum,&df);
			//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului 
			PowerFrequencyEstimate(autoSpectrum,nrPuncte/2,-1.0,winConst,df,7,&freqPeak,&powerPeak);
			SetCtrlVal( acqPanel, ACQ_PANEL_FREQ_PEAK, freqPeak);
			SetCtrlVal( acqPanel, ACQ_PANEL_POWER_PEAK, powerPeak);
	    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
			//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
		 	SpectrumUnitConversion(autoSpectrum, nrPuncte/2,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
			//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);	
			//plotez spectrul semnalului
	    	PlotWaveform( acqPanel,ACQ_PANEL_GRAPH_SPECTRU, convertedSpectrum, nrPuncte/2,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
	                             VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			
			aplicaFiltru();
			afisareFereastra();
			aplicaSpetruPerTotal();
			if(nrFereasra==numarFerestre)
			{
				nrFereasra=0;
			}
			else{
				nrFereasra++;
			}
			break;
	}
	return 0;
}
int afisarePerSecunda(int panel)
{
				GetCtrlVal(panel,ACQ_PANEL_SECUNDE,&secunda);
				WindowConst winConst;
				double *convertedSpectrum;//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
				double powerPeak = 0.0;//valoarea maxima din spectru de putere (din autoSpectrum)
				double freqPeak = 0.0;//variabila ce reprezinta pasul in domeniul frecventei
				double df = 0.0;//voltage signal - descriere a semnalului achizitionat
				char unit[32] = "V";//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
				double *autoSpectrum;
				dim=waveInfo[NPOINTS]/6;
				convertedSpectrum=(double*)calloc(dim/2,sizeof(double));
				autoSpectrum=(double*)calloc(dim/2,sizeof(double));
			
				vectForSeconds = (double *) calloc(dim, sizeof(double));
				int i;
				for(i=0;i<dim;i++)
				{
					vectForSeconds[i]=waveData[secunda*dim+i];
				}
				DeleteGraphPlot(acqPanel,ACQ_PANEL_WAVE_DATA,-1, VAL_IMMEDIATE_DRAW);
				PlotY(acqPanel,ACQ_PANEL_WAVE_DATA,vectForSeconds,dim,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
				
				ScaledWindowEx(vectForSeconds,dim,RECTANGLE_,0,&winConst);
				AutoPowerSpectrum(vectForSeconds,dim,1.0/sampleRate,autoSpectrum,&df);
				//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului 
				PowerFrequencyEstimate(autoSpectrum,dim/2,-1.0,winConst,df,7,&freqPeak,&powerPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_FREQ_PEAK, freqPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_POWER_PEAK, powerPeak);
		    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
				//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			 	SpectrumUnitConversion(autoSpectrum, dim/2,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
				
				//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
				DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);	
				//plotez spectrul semnalului
		    	PlotWaveform( acqPanel,ACQ_PANEL_GRAPH_SPECTRU, convertedSpectrum, dim/2,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
		                             VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
				
				
				aplicaFiltru();
				afisareFereastra();
				aplicaSpetruPerTotal();
				
				return 0;
}
int afisareSemnalComplet()
{
				WindowConst winConst;
				double *convertedSpectrum;//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
				double powerPeak = 0.0;//valoarea maxima din spectru de putere (din autoSpectrum)
				double freqPeak = 0.0;//variabila ce reprezinta pasul in domeniul frecventei
				double df = 0.0;//voltage signal - descriere a semnalului achizitionat
				char unit[32] = "V";//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
				double *autoSpectrum;
				convertedSpectrum=(double*)calloc(npoints/2,sizeof(double));
				autoSpectrum=(double*)calloc(npoints/2,sizeof(double));
				DeleteGraphPlot(acqPanel,ACQ_PANEL_WAVE_DATA,-1, VAL_IMMEDIATE_DRAW);
				PlotY(acqPanel,ACQ_PANEL_WAVE_DATA,waveData,npoints,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_CYAN);
				
				ScaledWindowEx(waveData,npoints,RECTANGLE_,0,&winConst);
				AutoPowerSpectrum(waveData,npoints,1.0/sampleRate,autoSpectrum,&df);
				//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului 
				PowerFrequencyEstimate(autoSpectrum,npoints/2,-1.0,winConst,df,7,&freqPeak,&powerPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_FREQ_PEAK, freqPeak);
				SetCtrlVal( acqPanel, ACQ_PANEL_POWER_PEAK, powerPeak);
		    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
				//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			 	SpectrumUnitConversion(autoSpectrum, npoints/2,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
				
				//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
				DeleteGraphPlot (acqPanel, ACQ_PANEL_GRAPH_SPECTRU, -1, VAL_IMMEDIATE_DRAW);	
				//plotez spectrul semnalului
		    	PlotWaveform( acqPanel,ACQ_PANEL_GRAPH_SPECTRU, convertedSpectrum, npoints/2,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
		                             VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
				
				aplicaFiltru();
				afisareFereastra();
				aplicaSpetruPerTotal();
					return 0;
}
	
int CVICALLBACK SelectareTipAfisare (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(acqPanel,ACQ_PANEL_TIP_AFISARE,ATTR_CTRL_INDEX,&tipAfisare);
			afisare=1;
			if(tipAfisare==0)//afisare per nr puncte
			{
				nrFereasra=0;
				SetCtrlAttribute(panel, ACQ_PANEL_TIMER, ATTR_ENABLED, 1);
				SetCtrlAttribute(panel, ACQ_PANEL_SECUNDE, ATTR_DIMMED, 1);
				SetCtrlAttribute(panel, ACQ_PANEL_NUMAR_PUNCTE, ATTR_DIMMED, 0);
			}
			if(tipAfisare==1)//afisare pe secunde
			{
				SetCtrlAttribute(panel,ACQ_PANEL_TIMER,ATTR_ENABLED,0);
				SetCtrlAttribute(panel, ACQ_PANEL_SECUNDE, ATTR_DIMMED, 0);
				SetCtrlAttribute(panel, ACQ_PANEL_NUMAR_PUNCTE, ATTR_DIMMED, 1);
				afisarePerSecunda(panel);
			}
			if(tipAfisare==2)//afisare tot semnalul
			{
				SetCtrlAttribute(panel,ACQ_PANEL_TIMER,ATTR_ENABLED,0);
				SetCtrlAttribute(panel, ACQ_PANEL_SECUNDE, ATTR_DIMMED, 1);
				SetCtrlAttribute(panel, ACQ_PANEL_NUMAR_PUNCTE, ATTR_DIMMED, 1);
				afisareSemnalComplet();
			}
			break;
	}
	return 0;
}

int CVICALLBACK schimbareSec (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(afisare)
			{
				afisarePerSecunda(panel);
			}
			break;
	}
	return 0;
}
int CVICALLBACK SetFilterType (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(acqPanel,ACQ_PANEL_FILTER_TYPE,ATTR_CTRL_INDEX, &TipFiltru);
			if(afisare)
			{
				aplicaFiltru();
			}
			break;
	}
	return 0;
}

int CVICALLBACK SetWindowType (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute(acqPanel,ACQ_PANEL_WINDOW_TYPE,ATTR_CTRL_INDEX,&TipFereastra);
			if(afisare)
			{
				afisareFereastra();
			}
			break;
	}
	return 0;
}


int CVICALLBACK OnSave2 (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int bitmapId=0;
			GetCtrlDisplayBitmap(panel,ACQ_PANEL_GRAPH_SPECTRU,1,&bitmapId);
			char*filename=0;
			filename=(char*)calloc(100,sizeof(char));
			//spectru simplu
			if(tipAfisare==0)
			{
				sprintf(filename, "Spectru_simplu_wav_1_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
			}
			if(tipAfisare==1)
			{
				sprintf(filename, "Spectru_simplu_wav_%d_%d.png", secunda,secunda+1);
			}
			if(tipAfisare==2)
			{
				sprintf(filename, "Spectr_simplu_wav_full.png");
			}
			//fereastra simpla
			SaveBitmapToJPEGFile(bitmapId,filename,JPEG_DCTFLOAT,50);
			DiscardBitmap(bitmapId);
			GetCtrlDisplayBitmap(panel,ACQ_PANEL_GRAPH_WINDOW,1,&bitmapId);
			if(tipAfisare==0)
			{
				if(TipFereastra==0)
				{
					sprintf(filename, "fereastra_simpla_hamming_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
				}
				else if(TipFereastra==1)
				{
					sprintf(filename, "fereastra_simpla_hanning_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
				}
			}
			if(tipAfisare==1)
			{
				if(TipFereastra==0)
				{
					sprintf(filename, "fereastra_simpla_hamming_perSec_%d_%d.png", secunda,secunda+1);
				}
				else if(TipFereastra==1)
				{
					sprintf(filename, "fereastra_simpla_hanning_perSec_%d_%d.png", secunda,secunda+1);
				}
			}
			if(tipAfisare==2)
			{
				if(TipFereastra==0)
				{
					sprintf(filename, "fereastra_simpla_hamming_full.png");
				}
				else if(TipFereastra==1)
				{
					sprintf(filename, "fereastra_simpla_hanning_full.png");
				}
			}
			SaveBitmapToJPEGFile(bitmapId,filename,JPEG_DCTFLOAT,50);
			DiscardBitmap(bitmapId);
			//semnal filtrat
			GetCtrlDisplayBitmap(panel,ACQ_PANEL_GRAPH_WAVE_FILTRED,1,&bitmapId);
			if(tipAfisare==0)
			{
				if(TipFiltru==0)
				{
					sprintf(filename, "semnal_filtrat_ksr_hpf_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
				}
				if(TipFiltru==1)
				{
					sprintf(filename, "semnal_filtrat_chebyshev_II_inv_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
				}
			}
			if(tipAfisare==1)
			{
				
				if(TipFiltru==0)
				{
					sprintf(filename, "semnal_filtrat_ksr_hpf_%d_%d.png",secunda,secunda+1);
				}
				if(TipFiltru==1)
				{
					sprintf(filename, "semnal_filtrat_chebyshev_II_inv_%d_%d.png",secunda,secunda+1);
				}
			}
			if(tipAfisare==2)
			{
				if(TipFiltru==0)
				{
					sprintf(filename, "semnal_filtrat_ksr_hpf_full.png");
				}
				if(TipFiltru==1)
				{
					sprintf(filename, "semnal_filtrat_chebyshev_II_inv_full.png");
				}
			}
			SaveBitmapToJPEGFile(bitmapId,filename,JPEG_DCTFLOAT,50);
			DiscardBitmap(bitmapId);
			//semnal ferestruit si filtrat
			GetCtrlDisplayBitmap(panel,ACQ_PANEL_WAVE_FERESTRUIT,1,&bitmapId);
			if(tipAfisare==0)
			{
				if(TipFereastra==0)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "semnal_ferestruit_hamming_ksr_hpf__perNrPct%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "semnal_ferestruit_hamming_chebyshev_II_inv_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
				}
				else if(TipFereastra==1)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "semnal_ferestruit_hanning_ksr_hpf_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "semnal_ferestruit_hanning_chebyshev_II_inv_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
				}
			}
			if(tipAfisare==1)
			{
				if(TipFereastra==0)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "semnal_ferestruit_hamming_ksr_hpf_perSec_%d_%d.png", secunda,secunda+1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "semnal_ferestruit_hamming_chebyshev_II_inv_perSec_%d_%d.png", secunda,secunda+1);
					}
				}
				else if(TipFereastra==1)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "semnal_ferestruit_hanning_ksr_hpf_perSec_%d_%d.png", secunda,secunda+1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "semnal_ferestruit_hanning_chebyshev_II_inv_perSec_%d_%d.png", secunda,secunda+1);
					}
				}
			}
			if(tipAfisare==2)
			{
				if(TipFereastra==0)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "semnal_ferestruit_hamming_ksr_hpf_full.png");
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "semnal_ferestruit_hamming_chebyshev_II_inv_full.png");
					}
				}
				else if(TipFereastra==1)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "semnal_ferestruit_hanning_ksr_hpf_full.png");
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "semnal_ferestruit_hanning_chebyshev_II_inv_full.png");
					}
				}
				
			}
			SaveBitmapToJPEGFile(bitmapId,filename,JPEG_DCTFLOAT,50);
			DiscardBitmap(bitmapId);
			
			GetCtrlDisplayBitmap(panel,ACQ_PANEL_GRAPH_SPECTR_FILT_FER,1,&bitmapId);
		
			if(tipAfisare==0)
			{
				if(TipFereastra==0)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hamming_ksr_hpf__perNrPct%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hamming_chebyshev_II_inv_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
				}
				else if(TipFereastra==1)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hanning_ksr_hpf_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hanning_chebyshev_II_inv_perNrPct_%d_%d.png", nrFereasra*nrPuncte,(nrFereasra+1)*nrPuncte-1);
					}
				}
			}
			if(tipAfisare==1)
			{
				if(TipFereastra==0)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hamming_ksr_hpf_perSec_%d_%d.png", secunda,secunda+1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hamming_chebyshev_II_inv_perSec_%d_%d.png", secunda,secunda+1);
					}
				}
				else if(TipFereastra==1)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hanning_ksr_hpf_perSec_%d_%d.png", secunda,secunda+1);
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hanning_chebyshev_II_inv_perSec_%d_%d.png", secunda,secunda+1);
					}
				}
			}
			if(tipAfisare==2)
			{
				if(TipFereastra==0)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hamming_ksr_hpf_full.png");
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hamming_chebyshev_II_inv_full.png");
					}
				}
				else if(TipFereastra==1)
				{
					if(TipFiltru==0)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hanning_ksr_hpf_full.png");
					}
					if(TipFiltru==1)
					{
						sprintf(filename, "spectrul_semnalului_filtrat_ferestruit_hanning_chebyshev_II_inv_full.png");
					}
				}
				
			}
			SaveBitmapToJPEGFile(bitmapId,filename,JPEG_DCTFLOAT,50);
			DiscardBitmap(bitmapId);
			free(filename);
			break;
	}
	return 0;
}
