#pragma once
#include <vector>
#include <mutex>
#include <utility>	
#include <map>

#include "Tableau3D.h"
#include "ImageClasse.h"
#include "ImageGeneralTools.h"


std::vector<std::string> listeDirToTableau(std::string dirDB);

class CPuzzleSolveur
{
	std::mutex m_mutexVecHisto;

	std::vector<std::string> m_vPiecesImageName;

	const int m_cQuantificationHisto = 16;
	S_HistoCouleur m_histoCouleurPuzzleComplet;
	std::vector<S_HistoCouleur> m_histosCouleurPiecesInde;

	std::vector<SIGNATURE_Forme> m_vPiecesHough;

	std::map<std::string, std::pair<SPoint2d, SPoint2d>> m_assocPieceROI;

public:
	CPuzzleSolveur() = default;

	void pretraitementPiecesIndependantes(std::string & pathPiecesUniques);
	void pretraitementPuzzleComplet(std::string & puzzleCompletPath);
	void recherchePositionPiecesDansPuzzle(); // calcul proba + detect_max + choix du ROI en fonction de Hough

	std::pair<SPoint2d, SPoint2d> getROIFromPieceName(std::string pieceName);

private:
	void segmentePieceIndependante(std::vector<std::string> & fileImage2Process);
	std::pair<SPoint2d, SPoint2d> placePieceDansPuzzle(SPoint2d pointMaxProba);
};