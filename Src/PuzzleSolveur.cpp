#include "PuzzleSolveur.h"

#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>

// Personnelles
#include "ImageDouble.h"
#include "ImageCouleur.h"
#include "TransformeeHough.h"

void CPuzzleSolveur::segmentePieceIndependante(std::vector<std::string> & fileImage2Process)
{
	int quantificationHisto = 16;

	for (auto it = fileImage2Process.begin(); it != fileImage2Process.end(); it++)
	{
		std::vector<S_RGB> pixelsSegmentes;
		S_RGB currPx;
		CImageCouleur pieceSrc(*it);

		CImageNdg imgSegmentee = CImageDouble(pieceSrc.plan(), "cast").filtrage("median", 10).toNdg();
		imgSegmentee = imgSegmentee.seuillage("manuel", 40, 255);

		for (int x = 0; x < imgSegmentee.lireHauteur() * imgSegmentee.lireLargeur(); x++)
		{
			// Pour faire le masque sur l'image originale
			//pieceSrc(x)[0] = pieceSrc(x)[0] * imgSegmentee(x) / 255;
			//pieceSrc(x)[1] = pieceSrc(x)[1] * imgSegmentee(x) / 255;
			//pieceSrc(x)[2] = pieceSrc(x)[2] * imgSegmentee(x) / 255;

			if (imgSegmentee(x) == 255)
			{
				currPx.valueR = pieceSrc(x)[0];
				currPx.valueG = pieceSrc(x)[1];
				currPx.valueB = pieceSrc(x)[2];

				pixelsSegmentes.emplace_back(currPx);
			}
		}
		
		// Cr�ation d'une image fictive, poss�dant uniquement les pixels de l'objet consid�r� (ici la pi�ce de puzzle) pour calculer l'histogramme couleur uniquement sur l'objet
		CImageCouleur fausseImageObjet(1, pixelsSegmentes.size(), pixelsSegmentes);

		m_mutexVecHisto.lock();
		m_histosCouleurPiecesInde.push_back(fausseImageObjet.histogrammeCouleur(quantificationHisto));
		m_mutexVecHisto.unlock();
	}
}

void CPuzzleSolveur::pretraitementPiecesIndependantes(std::string & pathPiecesUniques)
{
	m_vPiecesImageName = listeDirToTableau(pathPiecesUniques);

	// Multithreading assist�
	// Chaque pr�-traitement sur les pi�ces uniques de puzzle peut �tre trait� ind�pendemment des autres pi�ces
	// Plusieurs impl�mentation du multithreading ont �t� programm�es et test�es :
	//		- Choix du nombre de thread + cr�ation des thread automatiquement, traitant une file unique (concurrence g�r�e � travers mutex) ;
	//		- M�thode manuelle (ci-dessous) pour avoir 4 threads, traitant 4 tableaux s�par�s (pas de concurrence).
	//	+---------------+-----------------+-----------------+--------------------+--------------------+
	//	| S�quentiel	| 2 threads - 2[] | 4 threads - 4[] | 2 threads - 1 pile | 4 threads - 1 pile |
	//	+---------------+-----------------+-----------------+--------------------+--------------------+
	//	| 7.5 s			| 4.2 s			  | 2.4	s			| 5.7 s				 | 5.7 s			  |
	//	+---------------+-----------------+-----------------+--------------------+--------------------+
	// Compte tenu du peu d'images du puzzle et des r�sultats obtenus, nous avons choisi la m�thode manuelle

	int nbThread = 9; // 9 thread -> 45 % 9 -> Reste 0, chaque thread s'occupe de 5 images 
	int nbImageParThread = m_vPiecesImageName.size() / nbThread;

	std::cout << "Lancement du pr�traitement des pi�ces ind�pendantes : flou m�dian, seuillage et histogramme couleur objet - " << nbThread << " threads." << std::endl;
	auto start = std::chrono::steady_clock::now();

	std::vector<std::vector<std::string>> sousEnsembleImg(nbThread);
	std::vector<std::thread> vThread;
	int kT;
	for (kT = 0; kT < nbThread - 1; kT++)
	{
		sousEnsembleImg.at(kT) = (std::vector<std::string>(m_vPiecesImageName.end() - (kT + 1) * nbImageParThread, m_vPiecesImageName.end() - kT * nbImageParThread));
		vThread.emplace_back([&]() { segmentePieceIndependante(sousEnsembleImg.at(kT)); });
	}
	sousEnsembleImg.at(kT) = std::vector<std::string>(m_vPiecesImageName.begin(), m_vPiecesImageName.end() - kT * nbImageParThread); //listeImagesPieceTab.begin() + nbImageParThread + 1
	vThread.emplace_back([&]() { segmentePieceIndependante(sousEnsembleImg.back()); });

	for (auto& thread : vThread)
	{
		thread.join();
	}

	auto end = std::chrono::steady_clock::now();
	std::cout << "Pr�traitement des images des pi�ces termin� (" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms)" << std::endl;
}

void CPuzzleSolveur::pretraitementPuzzleComplet(std::string & puzzleCompletPath)
{
	CImageCouleur imgPuzzleComplet(puzzleCompletPath);

	std::cout << "Lancement du pr�traitement du puzzle : d�tection de bords (vec. gradient), d�tection des limites de pi�ces (Hough lin.) et histogramme couleur - possibilite de threading" << std::endl;

	m_histoCouleurPuzzleComplet = imgPuzzleComplet.histogrammeCouleur(16);

	auto start = std::chrono::steady_clock::now();
	// Partie recherche des pi�ces dans le puzzle
	std::shared_ptr<CImageNdg>imgSrcEdges = std::make_shared<CImageNdg>(CImageDouble(imgPuzzleComplet.plan(), "cast").vecteurGradient().toNdg());

	imgSrcEdges = std::make_shared<CImageNdg>(imgSrcEdges->seuillage("manuel", 40, 200));
	imgSrcEdges->sauvegarde("resFiltrage");

	feature_measurement::CTransformationHough myHoughT(0, 180, 90);
	myHoughT.calculeTransformee(imgSrcEdges);
	myHoughT.extraitAccumulations(450);

	auto houghOut = myHoughT.dessineHoughResultat("carte");

	CImageClasse piecesSeg(houghOut, "V4");
	m_vPiecesHough = piecesSeg.filtrage("taille", 70000, true).sigComposantesConnexes(true); // La surface d'une piece dans l'image > 70000 px

	houghOut.sauvegarde("HoughMap");

	auto end = std::chrono::steady_clock::now();
	std::cout << "Pr�traitement des images des pi�ces termin� (" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms)" << std::endl;
}

void CPuzzleSolveur::recherchePositionPiecesDansPuzzle()
{
	for (int i = 0; i < m_vPiecesImageName.size(); i++)
	{
		// TODO : 
		// - Calcule la carte de proba pour la pi�ce courante et le Puzzle
		// - Recherche point Baptiste

		auto retourPtBaptiste = sPoint();

		m_assocPieceROI.emplace(std::make_pair(m_vPiecesImageName.at(i), placePieceDansPuzzle(retourPtBaptiste)));
	}
}

std::pair<sPoint, sPoint> CPuzzleSolveur::getROIFromPieceName(std::string pieceName)
{
	auto search = m_assocPieceROI.find(pieceName);
	
	if (search != m_assocPieceROI.end()) 
	{
		std::cout << "Piece trouvee : " << "(" << search->second.first.x << "," << search->second.first.y << ")" << "(" << search->second.second.x << "," << search->second.second.y << ")" << std::endl;
	}
	else {
		std::cout << "Piece non trouvee" << std::endl;
	}

	return search->second;
}

std::pair<sPoint, sPoint> CPuzzleSolveur::placePieceDansPuzzle(sPoint pointMaxProba)
{
	std::pair<sPoint, sPoint> ROI;

	for (int i = 0; i < m_vPiecesHough.size(); i++)
	{
		// Si le point de probabilit� max est compris dans la pi�ce de Hough courante
		if (pointMaxProba.x >= m_vPiecesHough.at(i).rectEnglob_Hi
			&& pointMaxProba.x <= m_vPiecesHough.at(i).rectEnglob_Bi
			&& pointMaxProba.y >= m_vPiecesHough.at(i).rectEnglob_Hj
			&& pointMaxProba.y <= m_vPiecesHough.at(i).rectEnglob_Bj)
		{
			sPoint pt1ROI, pt2ROI;
			pt1ROI.x = m_vPiecesHough.at(i).rectEnglob_Hi;
			pt1ROI.y = m_vPiecesHough.at(i).rectEnglob_Hj;
			pt2ROI.x = m_vPiecesHough.at(i).rectEnglob_Bi;
			pt2ROI.y = m_vPiecesHough.at(i).rectEnglob_Bj;

			ROI = std::make_pair(pt1ROI, pt2ROI);
		}
	}

	return ROI;
}

std::vector<std::string> listeDirToTableau(std::string dirDB)
{
	std::string nomFichierFilename = "listePieces.txt";

	std::vector<std::string> listeNom;
	std::string ligne;

	std::string commandeDOS = "dir /B \"" + dirDB + "\\*.bmp\" > \"" + dirDB + "\\" + nomFichierFilename;
	system(commandeDOS.c_str()); // Execution de la commande DOS

	std::ifstream fichier(dirDB + "\\" + nomFichierFilename);

	if (fichier)
	{
		while (getline(fichier, ligne)) //Tant qu'on n'est pas a la fin, on lit
		{
			listeNom.emplace_back(dirDB + "\\" + ligne); // Cr�e la string (rvalue) directement dans la file
		}

		fichier.close();
	}
	else
	{
		std::cout << "Probleme a l'ouverture du fichier contenant le nom des pi�ces" << std::endl;
	}

	return listeNom;
}