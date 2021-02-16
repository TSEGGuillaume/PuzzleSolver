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

extern "C"
{
	_declspec(dllexport) CPuzzleSolveur * createPuzzleSolveur(char * pDirPiecesInde, char * pPathPuzzle)
	{
		CPuzzleSolveur * myPuzzleSolveur = new CPuzzleSolveur();
		std::string dirPiecesInde(pDirPiecesInde);
		std::string pathPuzzle(pPathPuzzle);

		myPuzzleSolveur->pretraitementPiecesIndependantes(dirPiecesInde);
		myPuzzleSolveur->pretraitementPuzzleComplet(pathPuzzle);

		myPuzzleSolveur->recherchePositionPiecesDansPuzzle();

		return myPuzzleSolveur;
	}
}

extern "C"
{
	_declspec(dllexport) int get_X1(CPuzzleSolveur * solver, char * imagette)
	{
		std::string myPieceName(imagette);
		return solver->getROIFromPieceName(myPieceName).first.x;
	}
}

extern "C"
{
	_declspec(dllexport) int get_Y1(CPuzzleSolveur * solver, char * imagette)
	{
		std::string myPieceName(imagette);
		return solver->getROIFromPieceName(myPieceName).first.y;
	}
}

extern "C"
{
	_declspec(dllexport) int get_X2(CPuzzleSolveur * solver, char * imagette)
	{
		std::string myPieceName(imagette);
		return solver->getROIFromPieceName(myPieceName).second.x;
	}
}

extern "C"
{
	_declspec(dllexport) int get_Y2(CPuzzleSolveur * solver, char * imagette)
	{
		std::string myPieceName(imagette);
		return solver->getROIFromPieceName(myPieceName).second.y;
	}
}


void main()
{
	std::string dirPiecesInde = "..\\Images\\pieces";
	std::string pathPuzzleComplet = "..\\Images\\PuzzleCompletBase.bmp";

	CPuzzleSolveur myPuzzleSolveur;

	// Prétraitement des pièces indépendantes
	myPuzzleSolveur.pretraitementPiecesIndependantes(dirPiecesInde);
	// Prétraitement du puzzle complet
	myPuzzleSolveur.pretraitementPuzzleComplet(pathPuzzleComplet);
	myPuzzleSolveur.recherchePositionPiecesDansPuzzle();

	int numImg = 0;
	while (true)
	{
		std::cin >> numImg;
		myPuzzleSolveur.getROIFromPieceName("..\\Images\\pieces\\" + std::to_string(numImg) + ".bmp");
	}

	system("pause");
}