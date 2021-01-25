// ajouter _declspec(dllexport) devant tout public pour permettre � la dll d'exporter ces m�thodes 
// pour qu'elles puissent �tre utilis�es par d'autres applications ou programmes

#pragma once

#ifndef _IMAGE_COULEUR_
#define _IMAGE_COULEUR_

// d�finition classe Image format bmp
#include <vector>
#include <string>
#include <algorithm>

#include "ImageNdg.h"

#define MIN3(x,y,z)  ((y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)))
#define MAX3(x,y,z)  ((y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)))

class CImageOpenCV;

class CImageCouleur 
{
	///////////////////////////////////////
private:
	///////////////////////////////////////

	int              m_iHauteur;
	int              m_iLargeur;
	std::string      m_sNom;
	unsigned char*   m_pucData;
	unsigned char**  m_ppucPixel;

	///////////////////////////////////////
public:
	///////////////////////////////////////

		// constructeurs
	_declspec(dllexport) CImageCouleur();
	_declspec(dllexport) CImageCouleur(int hauteur, int largeur, int valR = -1, int valV = -1, int valB = -1); // si -1 alors non pixels non initialis�s
	_declspec(dllexport) CImageCouleur(const std::string& nom);
	_declspec(dllexport) CImageCouleur(const CImageCouleur& im);
	_declspec(dllexport) CImageCouleur(const CImageNdg& im);
	_declspec(dllexport) CImageCouleur(const CImageNdg& pR, const CImageNdg& pG, const CImageNdg& pB);
	_declspec(dllexport) CImageCouleur(const CImageNdg& im, const CImageNdg& mask, int R = 255, int G = 0, int B = 0); // img mask binaire de pr�f�rence contours

	_declspec(dllexport) CImageCouleur(const CImageOpenCV& myMat, bool bgr2rgb = false); // Constructeur pour convertir une CImageOpenCV (d�riv�e de cv::Mat) vers CImageCouleur

	_declspec(dllexport) ~CImageCouleur(); // destructeur

	// sauvegarde au format bmp
	_declspec(dllexport) void sauvegarde(const std::string& file = ""); // sauvegarde data au format BMP

	// pouvoir acc�der � un pixel par image(i,j)[plan]
	_declspec(dllexport) unsigned char*& operator() (int i, int j) const {
		return m_ppucPixel[i*m_iLargeur + j];
	}

	// pouvoir acc�der � un pixel par image(i)[plan]
	_declspec(dllexport) unsigned char*& operator() (int i) const {
		return m_ppucPixel[i];
	}
	// op�rateur copie image par imOut = imIn
	_declspec(dllexport) CImageCouleur  &operator=(const CImageCouleur& im);

	// get et set 

	_declspec(dllexport) int lireHauteur() const {
		return m_iHauteur;
	}

	_declspec(dllexport) int lireLargeur() const {
		return m_iLargeur;
	}

	_declspec(dllexport) std::string lireNom() const {
		return m_sNom;
	}

	_declspec(dllexport) void ecrireHauteur(int hauteur) {
		m_iHauteur = hauteur;
	}

	_declspec(dllexport) void ecrireLargeur(int largeur) {
		m_iLargeur = largeur;
	}

	_declspec(dllexport) void ecrireNom(std::string nom) {
		m_sNom = nom;
	}

	_declspec(dllexport) int lireNbPixels() const {
		return m_iHauteur*m_iLargeur;
	}

	_declspec(dllexport) unsigned char * getData() { return m_pucData; }
		

	// histogramme = 3 x plans 
	_declspec(dllexport) std::vector<unsigned long> histogramme(bool enregistrementCSV = false); 

	// gestion des plans
	_declspec(dllexport) CImageNdg plan(int choix = 3, double poidsRouge = 1. / 3., double poidsVert = 1. / 3., double poidsBleu = 1. / 3.); // 0 -> R ou H, 1 -> V ou S, 2 -> B ou V et 4 -> luminance d'o� les poids fct de l'illuminant
	_declspec(dllexport) CImageCouleur& miseAJourPlan(int choix, const CImageNdg& plan);

	// changements d'espace couleur : HSV
	_declspec(dllexport) CImageCouleur conversion(const std::string& methode = "HSV") const; // accessible par les objets constants

	// transformation
	_declspec(dllexport) CImageCouleur transformation(const std::string& methode = "expansion"); // sortie sur [vMinOut,vMaxOut] pour expansion notamment sur chaque plan

	// masquage
	_declspec(dllexport) CImageCouleur masquage(const CImageNdg& im, const CImageNdg& mask, int R=255, int G=0, int B=0); // img mask binaire de pr�f�rence contours

};

#endif _IMAGE_COULEUR_