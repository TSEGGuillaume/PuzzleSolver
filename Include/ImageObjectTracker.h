#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include "ImageNdg.h"
#include "ImageCouleur.h"
#include "ImageDouble.h"
#include "Tableau3D.h"
#include "ImageGeneralTools.h"

SPoint2d detecteMaxCarteProba(const std::vector<double> & carteProbabilite, int tailleX, int tailleY, int x, int y)
{
	SPoint2d centreBestROI;

	// Détermination du rectangle englobant des centres de fenêtres
	int Xfp = (tailleX - 1) / 2;
	int Yfp = (tailleY - 1) / 2;
	int Xlp = x - (tailleX - 1) / 2;
	int Ylp = y - (tailleY - 1) / 2;	

	// Calcul de la moyenne des probabilités sur la fenêtre pour chaque centre

	double val = 0.0;
	int count = 0;
	std::vector<double> temp(carteProbabilite.size(), 0);

	for (int j = Yfp; j < Ylp; j+=(tailleY*1.0f/2))
	{
		for (int i = Xfp; i < Xlp; i+=(tailleX*1.0f / 2))
		{
			for (int l = j - (tailleY - 1) / 2; l < j + (tailleY - 1) / 2; l++)
			{
				for (int m = i - (tailleX - 1) / 2; m<i + (tailleX - 1) / 2; m++)
				{
					count++;
					val += carteProbabilite.at(l*x + m);
				}
			}

			val = val / count;
			temp.at(i + j * x) = val;
			val = 0;
			count = 0;
		}
	}

	// Détermination du centre ayant la proba max

	int indice = 0;

	for (int j = Yfp; j < Ylp; j += (tailleY*1.0f / 2))
	{
		for (int i = Xfp; i < Xlp; i += (tailleX*1.0f / 2))
		{
			if (temp.at(indice) < temp.at(j*x + i))
			{
				indice = i + j * x;
			}
		}
	}
	
	centreBestROI.x = indice % y;
	centreBestROI.y = static_cast<int>(indice * 1.0f / x);

	return centreBestROI;
}

std::vector<double> carteProbabilite(const S_HistoCouleur & histo_img, const S_HistoCouleur & histo_object)
{
	if (histo_img.bin != histo_object.bin)
	{
		throw std::string("Impossible de comparer les histogrammes car leurs pas de quantification sont différents");
	}

	int nbBin = histo_img.bin; // Les pas de quantification sont identiques, on prend l'un ou l'autre...

	// Prob matrix creation
	CTableau3D<double> prob(nbBin, nbBin, nbBin);

	for (int binRed = 0; binRed < nbBin; binRed++)
	{
		for (int binGreen = 0; binGreen < nbBin; binGreen++)
		{
			for (int binBlue = 0; binBlue < nbBin; binBlue++)
			{
				prob.at(binRed, binGreen, binBlue) = (histo_object.histogramme.at(binRed, binGreen, binBlue) + 1) / (histo_img.histogramme.at(binRed, binGreen, binBlue) + histo_object.histogramme.at(binRed, binGreen, binBlue) + 2);
			}
		}
	}

	// Prob map vector and image
	std::vector<double> carte_proba_vec;

	for (int it_pixel = 0; it_pixel < histo_img.idx_histogramme.size(); it_pixel++)
	{
		double proba_value = prob.at(histo_img.idx_histogramme.at(it_pixel));
		carte_proba_vec.push_back(proba_value);
	}

	return carte_proba_vec;
}