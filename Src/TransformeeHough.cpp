#pragma once
#include <iostream>
#include <fstream>

#include "TransformeeHough.h"

feature_measurement::CTransformationHough::CTransformationHough()
	: m_angleRechercheMin{ 0 }, m_angleRechercheMax{ 180 }, m_angleStep{ 1 }, 
	m_seuilDetection{ 1000 },
	m_largeurAccumulateur{ 0 }, m_hauteurAccumulateur{ 0 }
{
}

feature_measurement::CTransformationHough::CTransformationHough(int degreMin, int degreMax, int degreStep)
{
	m_angleRechercheMin = degreMin;
	m_angleRechercheMax = degreMax;
	m_angleStep = degreStep;
}

feature_measurement::CTransformationHough::~CTransformationHough()
{
	m_spLastTransformedImg.reset();
}

void feature_measurement::CTransformationHough::calculeTransformee(std::shared_ptr<CImageNdg> imgSrc)
{
	// Pour garder une trace des images traitées ; utile pour les accesseurs
	m_spLastTransformedImg.reset();
	m_spLastTransformedImg = imgSrc;

	int hauteurImg = imgSrc->lireHauteur();
	int largeurImg = imgSrc->lireLargeur();
	double centerImgX = largeurImg * 1.0 / 2;
	double centerImgY = hauteurImg * 1.0 / 2;

	int seuilCalcule = 250; // NDG pour lequel on calcule

	// Création de l'accumulateur
	// L'accumulateur est une matrice de theta_max colonnes et rho_max lignes
	double hough_h = (sqrt(2.0) * static_cast<double>(hauteurImg > largeurImg ? hauteurImg : largeurImg)) / 2.0;
	
	m_largeurAccumulateur = 180;
	m_hauteurAccumulateur = hough_h * 2.0; // -r -> +r
	m_vAccumulateur.resize(m_hauteurAccumulateur * m_largeurAccumulateur, 0);

	for (int y = 0; y < hauteurImg; y++)
	{
		for (int x = 0; x < largeurImg; x++)
		{
			if (imgSrc->operator()(y, x) > seuilCalcule)
			{
				for (int t = m_angleRechercheMin; t < m_angleRechercheMax; t += m_angleStep)
				{
					double r = (x * 1.0 - centerImgX) * cos(t * m_DEG2RAD) + (y * 1.0 - centerImgY) * sin(t * m_DEG2RAD);

					int indice2Inc = static_cast<int>(round(r + hough_h) * 180.0) + t;
					m_vAccumulateur.at(indice2Inc)++; // Remplissage de l'accumulateur
				}
			}
		}
	}
}

void feature_measurement::CTransformationHough::extraitAccumulations(int seuil)
{
	m_seuilDetection = seuil;

	if (m_vAccumulateur.empty() == false)
	{	
		SPoint2d pt1, pt2;

		for (int r = 0; r < m_hauteurAccumulateur; r++)
		{
			for (int t = 0; t < m_largeurAccumulateur; t++)
			{
				if (m_vAccumulateur.at(r * m_largeurAccumulateur + t) >= m_seuilDetection)
				{
					//Is this point a local maxima (9x9)
					int max = m_vAccumulateur.at(r * m_largeurAccumulateur + t);
					for (int ly = -4; ly <= 4; ly++)
					{
						for (int lx = -4; lx <= 4; lx++)
						{
							if ((ly + r >= 0 && ly + r < m_hauteurAccumulateur) && (lx + t >= 0 && lx + t < m_largeurAccumulateur))
							{
								if (m_vAccumulateur.at((r + ly) * m_largeurAccumulateur + (t + lx)) > max)
								{
									max = m_vAccumulateur.at((r + ly) * m_largeurAccumulateur + (t + lx));
									ly = lx = 5;
								}
							}
						}
					}
					if (max > m_vAccumulateur.at(r * m_largeurAccumulateur + t))
						continue;

					if (t >= 45 && t <= 135)
					{
						//y = (r - x cos(t)) / sin(t)
						pt1.x = 0;
						pt1.y = (static_cast<double>(r - (m_hauteurAccumulateur * 1.0 / 2)) - ((pt1.x - (m_spLastTransformedImg->lireLargeur() * 1.0 / 2)) * cos(t * m_DEG2RAD))) 
							/ sin(t * m_DEG2RAD) + (m_spLastTransformedImg->lireHauteur() * 1.0 / 2);
						pt2.x = m_spLastTransformedImg->lireLargeur() - 0;
						pt2.y = (static_cast<double>(r - (m_hauteurAccumulateur * 1.0 / 2)) - ((pt2.x - (m_spLastTransformedImg->lireLargeur() * 1.0 / 2)) * cos(t * m_DEG2RAD))) 
							/ sin(t * m_DEG2RAD) + (m_spLastTransformedImg->lireHauteur() * 1.0 / 2);
					}
					else
					{
						//x = (r - y sin(t)) / cos(t);
						pt1.y = 0;
						pt1.x = (static_cast<double>(r - (m_hauteurAccumulateur * 1.0 / 2)) - ((pt1.y - (m_spLastTransformedImg->lireHauteur() * 1.0 / 2)) * sin(t * m_DEG2RAD))) 
							/ cos(t * m_DEG2RAD) + (m_spLastTransformedImg->lireLargeur() * 1.0 / 2);
						pt2.y = m_spLastTransformedImg->lireHauteur() - 0;
						pt2.x = (static_cast<double>(r - (m_hauteurAccumulateur * 1.0 / 2)) - ((pt2.y - (m_spLastTransformedImg->lireHauteur() * 1.0 / 2)) * sin(t * m_DEG2RAD))) 
							/ cos(t * m_DEG2RAD) + (m_spLastTransformedImg->lireLargeur() * 1.0 / 2);
					}

					m_vPtsLines.push_back(std::make_pair(pt1, pt2));

				}
			}
		}

		std::cout << "Nombre de lignes detectees : " << m_vPtsLines.size() << std::endl;
	}
}

void feature_measurement::CTransformationHough::dessineLigne(CImageNdg & img2draw, SPoint2d pt1, SPoint2d pt2)
{
	// Rappel équation de droite : y = ax+b
	// Définition du coefficient directeur

	float a = static_cast<float>(pt2.y - pt1.y) / (pt2.x - pt1.x);
	float b = pt1.y - pt1.x * a;

	if (isinf(a)) // Si le coeff. directeur infini, on tire droite verticale
	{
		for (int y = 0; y < img2draw.lireHauteur(); y++)
		{
			img2draw(y, pt1.x) = 0;
		}
	}
	else
	{
		for (int x = 0; x < img2draw.lireLargeur(); x++)
		{
			int y = static_cast<int>(a * x + b);

			if (y >= 0 && y < img2draw.lireHauteur())
			{
				img2draw(y, x) = 0;
			}
		}
	}
}

CImageNdg feature_measurement::CTransformationHough::dessineHoughResultat(std::string modeDessin)
{
	CImageNdg out;

	if (modeDessin == "masque")
	{
		out = CImageNdg(*m_spLastTransformedImg);

		for (auto it = m_vPtsLines.begin(); it != m_vPtsLines.end(); it++)
		{
			SPoint2d pt1, pt2;
			pt1.x = it->first.x;
			pt1.y = it->first.y;
			pt2.x = it->second.x;
			pt2.y = it->second.y;

			dessineLigne(out, pt1, pt2);
		}
	}
	else if (modeDessin == "carte")
	{
		out = CImageNdg(m_spLastTransformedImg->lireHauteur(), m_spLastTransformedImg->lireLargeur(), 255);

		for (auto it = m_vPtsLines.begin(); it != m_vPtsLines.end(); it++)
		{
			SPoint2d pt1, pt2;
			pt1.x = it->first.x;
			pt1.y = it->first.y;
			pt2.x = it->second.x;
			pt2.y = it->second.y;

			dessineLigne(out, pt1, pt2);
		}
	}

	return out;
}
