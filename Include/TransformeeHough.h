#pragma once
#include <vector>
#include <memory>

#include "ImageNdg.h"

typedef struct STRUCT_POINTS_2D
{
	int x = -1;
	int	y = -1;
} SPoint2d;

// Définition d'un espace de nom qui contiendra les outils de mesure de caractéristique
namespace feature_measurement
{
	class CTransformationHough
	{
		const float m_DEG2RAD = 0.017453293f;

		int m_angleRechercheMin;
		int m_angleRechercheMax;
		int m_angleStep;

		int m_seuilDetection;
		std::shared_ptr<CImageNdg> m_spLastTransformedImg;

		std::vector<std::pair<SPoint2d, SPoint2d>> m_vPtsLines;

		std::vector<int> m_vAccumulateur;
		int m_largeurAccumulateur;
		int m_hauteurAccumulateur;

	public:
		CTransformationHough();
		CTransformationHough(int degreMin, int degreMax, int degreStep);
		~CTransformationHough();

		// Fonctions de traitement
		void calculeTransformee(std::shared_ptr<CImageNdg> imgSrc);
		void extraitAccumulations(int seuil);

		// Fonctions pour la visualisation
		CImageNdg dessineHoughResultat(std::string modeDessin = "masque");

		// Mutateurs et accesseurs
		inline void setAngleRechercheMin(int degreMin) { m_angleRechercheMin = degreMin; };
		inline void setAngleRechercheMax(int degreMax) { m_angleRechercheMax = degreMax; };
		inline void setAnglStep(int degreStep) { m_angleStep = degreStep; };

		inline int getAngleRechercheMin() const { return m_angleRechercheMin; };
		inline int getAngleRechercheMax() const { return m_angleRechercheMax; };
		inline int getAnglStep() const { return m_angleStep; };
		inline int getSeuilDetection() const { return m_seuilDetection; };

		inline std::vector<std::pair<SPoint2d, SPoint2d>> getLignes() const { return m_vPtsLines; };
		inline CImageNdg getLastProcessedImg() const { return *m_spLastTransformedImg; };

	private:
		void dessineLigne(CImageNdg & img2draw, SPoint2d pt1, SPoint2d pt2);

	};
}