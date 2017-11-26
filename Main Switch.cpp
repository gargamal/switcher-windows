/////////////////////////////////
//                             //
// Fichier source principale.  //
// Cr?? le 2 Octobre 2007, par //
// D PELLIER.                  //
/////////////////////////////////

#include <stdio.h>
#include <malloc.h>
#include <windows.h>

/////////////////////////////////////
// Proc?dures et fonctions locales //
/////////////////////////////////////
BOOL ChainePresente (char *Phrase, char *TexteAChercher);

//////////////////////////
// Proc?dure principale //
//////////////////////////
void main (void)
{
	HWND            Hwnd, HwndMem [255];
	int             NbHwnd;
	char            NomFenetre [255];
	char            MotClef    [255];
	int             i;
	int             TempoMilliSec;

	// Initialise les donn?es
	strcpy (MotClef, "Internet Explorer");

	printf ("Veuillez saisir la dur?e de la tempo en sec : ");
	if (scanf  ("%i", &TempoMilliSec) == 0) return ;
	TempoMilliSec = TempoMilliSec * 1000;
	NbHwnd = 0;

	Hwnd = FindWindow (NULL, NULL);

	// Recherche du nombre de handle avec un titre correspondant ? la chaine
	for (i = 0, NbHwnd = 0; Hwnd != NULL; i++)
	{
		GetWindowText (Hwnd, NomFenetre, 255);

		if (ChainePresente (NomFenetre, MotClef))
			HwndMem [NbHwnd++] = Hwnd;

		Hwnd = GetNextWindow (Hwnd, GW_HWNDNEXT);
	}

	// Premier passage on minimize toutes les fen?tres
	for (i = 0; i < NbHwnd; i++)
		ShowWindow (HwndMem [i], SW_MINIMIZE);
	// Boucle d'activation
	i = 0;
	while (1)
	{
		ShowWindow (HwndMem [i], SW_MAXIMIZE);

		Sleep (TempoMilliSec);

		ShowWindow (HwndMem [i], SW_MINIMIZE);
		i++;
		if (i == NbHwnd) i = 0;
	}
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
