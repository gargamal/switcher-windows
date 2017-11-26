/////////////////////////////////////
//                                 //
// Fichier source principal.       //
// Main.cpp cr?? le 2 Octobre 2006 //
// ?crit par D PELLIER.            //
/////////////////////////////////////

#include <windows.h>
#include "Resource.h"

////////////////////////////////////////
// En t?te des proc?dure et fonctions //
////////////////////////////////////////
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
BOOL ChainePresente (char *Phrase, char *TexteAChercher);
BOOL TesteCasse (char *Texte);

/////////////////////////
// Focntion Principale //
/////////////////////////
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
PSTR szCmdLine, int iCmdShow)
{
	static   TCHAR szAppName[] = TEXT ("Switcher");
	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_ICONAPPLICATION));
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) (COLOR_BTNFACE);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("Ce programme fonctionne exclusivement sous Windows NT !"),
		szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow (szAppName, szAppName, 
						WS_CAPTION | WS_SYSMENU,
						CW_USEDEFAULT, CW_USEDEFAULT,
						250, 180,
						NULL, NULL, hInstance, NULL);

	ShowWindow (hwnd, iCmdShow);

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return msg.wParam;
}

///////////////////////////////////////////////////////////////////////////
// Fonction qui permet de savoir si la chaine est pr?sent dans la phrase //
///////////////////////////////////////////////////////////////////////////
BOOL ChainePresente (char *Phrase, char *TexteAChercher)
{
	unsigned int i, j;

	if (Phrase == NULL || TexteAChercher == NULL) return FALSE;

	for (i = 0, j = 0; i < strlen (Phrase); i++)
	{
		if (Phrase [i] == TexteAChercher [j])
		{
			j++;
			if (j == strlen (TexteAChercher)) return TRUE;
		}
		else j = 0;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Permet de tester la casse en v?rifiant qu'il s'agit d'un nombre //
/////////////////////////////////////////////////////////////////////
BOOL TesteCasse (char *Texte)
{
	UINT i;

	for (i = 0; i < strlen (Texte); i++)
	{
		if (!(Texte [i] == '0' || Texte [i] == '1' || Texte [i] == '2' || Texte [i] == '3'
		|| Texte [i] == '4' || Texte [i] == '5' || Texte [i] == '6' || Texte [i] == '7'
		|| Texte [i] == '8' || Texte [i] == '9')) return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////
// Proc?dure pour la gestion des ?v?nements //
//////////////////////////////////////////////
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hWndTempo;
	static HWND hWndOk;
	static HWND hWndAnnuler;
	static int  cxChar, cxCaps, cyChar;
	static UINT TopHorloge, Position, PositionAvant;
	static BOOL TicTac;
	static char MotClef [255];
	static HWND HwndMem [50];
	static UINT NbHwnd;
	HWND        hWndRecherche;
	PAINTSTRUCT ps;
	HDC         hdc;
	TEXTMETRIC  tm;
	char        Texte [255];
	UINT        i;

	switch (message)
	{
	case WM_CREATE:
		TicTac = FALSE;

		hdc = GetDC (hwnd);

		GetTextMetrics (hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC (hwnd, hdc);

		hWndTempo   = CreateWindow ("EDIT", "15", 
						WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
						125 - cxChar * 4, cyChar * 3, cxChar * 8, cyChar * 3 / 2, hwnd, (HMENU) 1,
						((LPCREATESTRUCT) lParam)->hInstance, NULL);

		hWndOk      = CreateWindow ("BUTTON", "Lancer", 
						WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
						10, 100, cxChar * 12, cyChar * 2, hwnd, (HMENU) 2,
						((LPCREATESTRUCT) lParam)->hInstance, NULL);

		hWndAnnuler = CreateWindow ("BUTTON", "Terminer", 
						WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
						235 - cxChar * 12, 100, cxChar * 12, cyChar * 2, hwnd, (HMENU) 3,
						((LPCREATESTRUCT) lParam)->hInstance, NULL);

		strcpy (MotClef, "Internet Explorer");
		Position = 0;

		return 0;

	case WM_PAINT:
		hdc = BeginPaint (hwnd, &ps);

		strcpy (Texte, "Entrez le nombre de secondes :");
		TextOut (hdc,  (250 - (strlen (Texte) * cxChar)) / 2, cyChar, 
					"Entrez le nombre de secondes :", strlen (Texte));

		EndPaint (hwnd, &ps);
		return 0;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case 1: // Texte Tempo
			GetWindowText (hWndTempo, Texte, 255);
			if (!TesteCasse (Texte))
			{
				strcpy (Texte, "0");
				SetWindowText (hWndTempo, Texte);
			}
			else
				TopHorloge = atoi (Texte) * 1000;
				
			break;

		case 2: // Bouton Lancer
			if (TicTac)
				KillTimer (hwnd, 1);

			hWndRecherche = FindWindow (NULL, NULL);
			TicTac = TRUE;

			// Recherche du nombre de handle avec un titre correspondant ? la chaine
			for (i = 0, NbHwnd = 0; hWndRecherche != NULL; i++)
			{
				GetWindowText (hWndRecherche, Texte, 255);

				if (ChainePresente (Texte, MotClef))
					HwndMem [NbHwnd++] = hWndRecherche;

				hWndRecherche = GetNextWindow (hWndRecherche, GW_HWNDNEXT);
			}
			// Premier passage on minimize toutes les fen?tres
			for (i = 0; i < NbHwnd; i++)
				ShowWindow (HwndMem [i], SW_MINIMIZE);

			SetTimer (hwnd, 1, TopHorloge, NULL);

			break;

		case 3: // Bouton Terminer
			SendMessage (hwnd, WM_DESTROY, 0, 0);
			break;
		}

		return 0;

	case WM_TIMER:
		ShowWindow (HwndMem [PositionAvant], SW_MINIMIZE);

		ShowWindow (HwndMem [Position], SW_MAXIMIZE);

		PositionAvant = Position;
		Position++;
		if (Position == NbHwnd) Position = 0;

		return 0;

	case WM_DESTROY:
		if (TicTac)
			KillTimer (hwnd, 1);
		PostQuitMessage (0);
		return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}
