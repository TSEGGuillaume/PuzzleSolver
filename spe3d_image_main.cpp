#pragma once

// STL
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <mutex>
#include <chrono>
#include <queue>
#include <algorithm>

#include <windows.h>

// Personnelles
#include "ImageDouble.h"
#include "ImageCouleur.h"
#include "TransformeeHough.h"

#include "PuzzleSolveur.h"

//std::queue<std::string> listeDirToFile(std::string dirDB)
//{
//	std::string nomFichierFilename = "listePieces.txt";
//
//	std::queue<std::string> listeNom;
//	std::string ligne;
//
//	std::string commandeDOS = "dir /B \"" + dirDB + "\\*.bmp\" > \"" + dirDB + "\\" + nomFichierFilename;
//	system(commandeDOS.c_str()); // Execution de la commande DOS
//
//	std::ifstream fichier(dirDB + "\\" + nomFichierFilename);
//
//	if (fichier)
//	{
//		while (getline(fichier, ligne)) //Tant qu'on n'est pas a la fin, on lit
//		{
//			listeNom.emplace(dirDB + "\\" + ligne); // Crée la string (rvalue) directement dans la file
//		}
//
//		fichier.close();
//	}
//	else
//	{
//		std::cout << "Probleme a l'ouverture du fichier contenant le nom des pièces" << std::endl;
//	}
//
//	return listeNom;
//}

//CImageNdg filtreMedianHisto(CImageNdg src, int rayon)
//{
//
//}


void main()
{
	std::string dirPiecesInde = "..\\Images\\pieces";
	std::string pathPuzzleComplet = "..\\Images\\PuzzleCompletBase.bmp";

	// Filtre médian approche histo
	CImageCouleur myOriginalImg("..\\Images\\pieces\\1.bmp");
	CImageNdg myOriginalNdg = myOriginalImg.plan();

	//CImageNdg res = filtreMedianHisto(myOriginalNdg, 10);


	CPuzzleSolveur myPuzzleSolveur;

	// Prétraitement des pièces indépendantes
	myPuzzleSolveur.pretraitementPiecesIndependantes(dirPiecesInde);
	// Prétraitement du puzzle complet
	myPuzzleSolveur.pretraitementPuzzleComplet(pathPuzzleComplet);

	system("pause");
}