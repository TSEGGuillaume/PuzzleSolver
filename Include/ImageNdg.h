// ajouter _declspec(dllexport) devant tout public pour permettre � la dll d'exporter ces m�thodes 
// pour qu'elles puissent �tre utilis�es par d'autres applications ou programmes

#pragma once

#ifndef _IMAGE_NDG_
#define _IMAGE_NDG_

// d�finition classe Image Ndg format bmp
#include <vector>
#include <string>
#include <algorithm>

#include "libImageEnC.h"

typedef struct {
	int		minNdg;
	int		maxNdg;
	int		medianeNdg;
	float	moyenneNdg;
	float	ecartTypeNdg;
} MOMENTS;

class CImageNdg {

	///////////////////////////////////////
	private : 
	///////////////////////////////////////

		int              m_iHauteur;
		int              m_iLargeur;  
		bool			 m_bBinaire;	
		std::string      m_sNom;
		unsigned char*   m_pucPixel;

		unsigned char*   m_pucPalette; 

	///////////////////////////////////////
	public : 
	///////////////////////////////////////

		// constructeurs
		_declspec(dllexport) CImageNdg(); // par d�faut
		_declspec(dllexport) CImageNdg(int hauteur, int largeur, int val=-1); // si -1 alors non pixels non initialis�s
		_declspec(dllexport) CImageNdg(const std::string& nom); // fichier en entr�e
		_declspec(dllexport) CImageNdg(const CImageNdg& im); // image en entr�e
		_declspec(dllexport) CImageNdg(const IMAGE& im); // image lib C en entr�e

		_declspec(dllexport) IMAGE toCImageNdg(); // transform to lib C 

		// destructeur
		_declspec(dllexport) ~CImageNdg(); 

		// sauvegarde au format bmp
		_declspec(dllexport) void sauvegarde(const std::string& file = ""); // sauvegarde data au format BMP

		// pouvoir acc�der � un pixel par image(i)
		_declspec(dllexport) unsigned char& operator() (int i) const { 
		return m_pucPixel[i];
		}

		// pouvoir acc�der � un pixel par image(i,j)
		_declspec(dllexport) unsigned char& operator() (int i, int j) const { 
		return m_pucPixel[i*m_iLargeur+j];
		}

		// op�rateur copie image par imOut = imIn
		_declspec(dllexport) CImageNdg& operator=(const CImageNdg& im);

		// get et set 

		_declspec(dllexport) int lireHauteur() const {  
		return m_iHauteur;
		}

		_declspec(dllexport) int lireLargeur() const {
		return m_iLargeur;
		}

		_declspec(dllexport) bool lireBinaire() const {
		return m_bBinaire;
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

		_declspec(dllexport) void ecrireBinaire(bool type) {
		m_bBinaire = type;
		}

		_declspec(dllexport) void ecrireNom(std::string nom) {
		m_sNom = nom;
		}

		_declspec(dllexport) int lireNbPixels() const { 
		return m_iHauteur*m_iLargeur;
		}

		_declspec(dllexport) CImageNdg& choixPalette(const std::string& name = "grise") {
			if (name.compare("rouge") == 0) {
				for (int indice=0;indice<256;indice ++) {
					this->m_pucPalette[indice*4]   = 0;
					this->m_pucPalette[indice*4+1] = 0;
					this->m_pucPalette[indice*4+2] = (unsigned char)(indice);
					this->m_pucPalette[indice*4+3] = 0;
				}
			}
			else
				if (name.compare("verte") == 0) {
					for (int indice=0;indice<256;indice ++) {
						this->m_pucPalette[indice*4]   = 0;
						this->m_pucPalette[indice*4+1] = (unsigned char)(indice);
						this->m_pucPalette[indice*4+2] = 0;
						this->m_pucPalette[indice*4+3] = 0;
					}
				}
				else
					if (name.compare("bleue") == 0) {
						for (int indice=0;indice<256;indice ++) {
							this->m_pucPalette[indice*4]   = (unsigned char)(indice);
							this->m_pucPalette[indice*4+1] = 0;
							this->m_pucPalette[indice*4+2] = 0;
							this->m_pucPalette[indice*4+3] = 0;
						}
					}
					else
						if (name.compare("binaire") == 0) {
							// fond toujours � 0
							this->m_pucPalette[0] = 0; this->m_pucPalette[1] = 0; this->m_pucPalette[2] = 0; this->m_pucPalette[3] = 0;

							// 8 couleurs de base
							// ensuite p�riodicit�, de sorte que l'objet 1 a le m�me label que l'objet 9 par exemple
							this->m_pucPalette[4]  = 255; this->m_pucPalette[5] = 0;    this->m_pucPalette[6] = 0;    this->m_pucPalette[7] = 0;
							this->m_pucPalette[8]  = 0;   this->m_pucPalette[9] = 255;  this->m_pucPalette[10] = 0;   this->m_pucPalette[11] = 0;
							this->m_pucPalette[12] = 0;   this->m_pucPalette[13] = 0;   this->m_pucPalette[14] = 255; this->m_pucPalette[15] = 0;
							this->m_pucPalette[16] = 255; this->m_pucPalette[17] = 255; this->m_pucPalette[18] = 0;   this->m_pucPalette[19] = 0;
							this->m_pucPalette[20] = 0;   this->m_pucPalette[21] = 255; this->m_pucPalette[22] = 255; this->m_pucPalette[23] = 0;
							this->m_pucPalette[24] = 255; this->m_pucPalette[25] = 0;   this->m_pucPalette[26] = 255; this->m_pucPalette[27] = 0;
							this->m_pucPalette[28] = 0;   this->m_pucPalette[29] = 128; this->m_pucPalette[30] = 255; this->m_pucPalette[31] = 0;
							this->m_pucPalette[32] = 128; this->m_pucPalette[33] = 255;   this->m_pucPalette[34] = 128; this->m_pucPalette[35] = 0;

							for (int indice=9;indice<256;indice ++) {
								this->m_pucPalette[indice*4]   = this->m_pucPalette[indice*4-32];
								this->m_pucPalette[indice*4+1] = this->m_pucPalette[indice*4+1-32];;
								this->m_pucPalette[indice*4+2] = this->m_pucPalette[indice*4+2-32];;
								this->m_pucPalette[indice*4+3] = 0;
							}
						}
						else // palette gris
							for (int indice=0;indice<256;indice ++) {
								this->m_pucPalette[indice*4]   = (unsigned char)(indice);
								this->m_pucPalette[indice*4+1] = (unsigned char)(indice);
								this->m_pucPalette[indice*4+2] = (unsigned char)(indice);
								this->m_pucPalette[indice*4+3] = 0;
							}
			return *this;
		}
		
		_declspec(dllexport) CImageNdg& choixPalette(std::vector<int>& map) { // couleurs donn�es par tableau de 256 couleurs RGB)
			for (int indice=0;indice<256;indice ++) {
				this->m_pucPalette[indice*4]   = (unsigned char)map[indice*3+2];
				this->m_pucPalette[indice*4+1] = (unsigned char)map[indice*3+1];
				this->m_pucPalette[indice*4+2] = (unsigned char)map[indice*3];
				this->m_pucPalette[indice*4+3] = 0;
			}
			return *this;
		}

		_declspec(dllexport) std::vector<int> lirePalette() const { // palette image sortie au format tableau de 256 couleurs RGB
			// palette image sortie au format tableau de 256 couleurs RGB
			std::vector<int> palette;
			palette.resize(256*3);
			for (int indice=0;indice<256;indice ++) {
				palette[indice*3+2] = this->m_pucPalette[indice*4];
				palette[indice*3+1] = this->m_pucPalette[indice*4+1];
				palette[indice*3] = this->m_pucPalette[indice*4+2];
			}
			return palette;
		}

		// histogramme
		_declspec(dllexport) std::vector<unsigned long> histogramme(bool enregistrementCSV = false); 

		// signatures
		_declspec(dllexport) MOMENTS signatures(const std::vector<unsigned long>& h);
		_declspec(dllexport) MOMENTS signatures();

		// op�rations ensemblistes pour images binaires
		_declspec(dllexport) CImageNdg& operation(const CImageNdg& im, const std::string& methode = "et");

		// seuillage
		_declspec(dllexport) CImageNdg seuillage(const std::string& methode = "otsu", int seuilBas = 128, int seuilHaut = 255); // seuillage automatique Otsu par d�faut, sinon "manuel" et choix des seuils Bas et Haut

		// transformation
		_declspec(dllexport) CImageNdg transformation(const std::string& methode = "complement"); 

};

#endif _IMAGE_NDG_