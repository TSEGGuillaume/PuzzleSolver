#pragma once
#include <vector>
#include <mutex>

#include "Tableau3D.h"

std::vector<std::string> listeDirToTableau(std::string dirDB);

class CPuzzleSolveur
{
	std::mutex m_mutexVecHisto;

	std::vector<std::string> m_vPiecesImageName;

	CTableau3D<double> m_histoCouleurPuzzleComplet;
	std::vector<CTableau3D<double>> m_histosCouleurPiecesInde;

public:
	CPuzzleSolveur() = default;

	void pretraitementPiecesIndependantes(std::string & pathPiecesUniques);
	void pretraitementPuzzleComplet(std::string & puzzleCompletPath);

private:
	void segmentePieceIndependante(std::vector<std::string> & fileImage2Process);
};