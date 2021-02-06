#pragma once
#include <vector>
#include <mutex>
#include <utility>	
#include <map>

#include "Tableau3D.h"
#include "ImageClasse.h"


std::vector<std::string> listeDirToTableau(std::string dirDB);

typedef struct STRUCT_POINT
{
	int x, y;
} sPoint;

class CPuzzleSolveur
{
	std::mutex m_mutexVecHisto;

	std::vector<std::string> m_vPiecesImageName;

	CTableau3D<double> m_histoCouleurPuzzleComplet;
	std::vector<CTableau3D<double>> m_histosCouleurPiecesInde;

	std::vector<SIGNATURE_Forme> m_vPiecesHough;

	std::map<std::string, std::pair<sPoint, sPoint>> m_assocPieceROI;

public:
	CPuzzleSolveur() = default;

	void pretraitementPiecesIndependantes(std::string & pathPiecesUniques);
	void pretraitementPuzzleComplet(std::string & puzzleCompletPath);
	void recherchePositionPiecesDansPuzzle(); // calcul proba + detect_max + choix du ROI en fonction de Hough

	std::pair<sPoint, sPoint> getROIFromPieceName(std::string pieceName);

private:
	void segmentePieceIndependante(std::vector<std::string> & fileImage2Process);
	std::pair<sPoint, sPoint> placePieceDansPuzzle(sPoint pointMaxProba);
};