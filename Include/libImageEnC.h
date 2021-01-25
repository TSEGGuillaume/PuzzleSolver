#pragma once

#define LIBIMAGEENC_API __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

/* librairie IPSI1 */

typedef struct image {
	int   Nblig;
	int   Nbcol;
	unsigned char*  data;
	unsigned char** pixel;
} IMAGE;

typedef struct rgb {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} RGB;

typedef struct imageRGB {
	int   Nblig;
	int   Nbcol;
	RGB*  data;
	RGB** pixel;
} IMAGERGB;

typedef struct signatures {
	int		min;
	int		max;
	double	moyenne;
	double	ecartType;
	int		mediane;
} SIGNATURES;

typedef struct signature_composantes_connexes {
	int surface;
	float perimetre;
	float compacite;
	int bord;
} SIGNATURE_COMPOSANTE_CONNEXE;

extern LIBIMAGEENC_API IMAGE allocationImage(int Nblig, int Nbcol);
extern LIBIMAGEENC_API void initialisationAleatoireImage(IMAGE img, int ngMin, int ngMax);

extern LIBIMAGEENC_API void sauvegardeImage(IMAGE img, const char *type, const char *out);
extern LIBIMAGEENC_API void liberationImage(IMAGE *im);

extern LIBIMAGEENC_API IMAGE lectureImage(const char *nom);

extern LIBIMAGEENC_API IMAGERGB allocationImageRGB(int Nblig, int Nbcol);
extern LIBIMAGEENC_API IMAGERGB lectureImageRGB(const char *nom);

extern LIBIMAGEENC_API void sauvegardeImageRGB(IMAGERGB img, const char *type, const char *out);
extern LIBIMAGEENC_API void liberationImageRGB(IMAGERGB *im);

extern LIBIMAGEENC_API IMAGE planImage(IMAGERGB img, int plan);
extern LIBIMAGEENC_API IMAGE luminanceImage(IMAGERGB img, float r, float g, float b);
extern LIBIMAGEENC_API IMAGE inverseImage(IMAGE img);
extern LIBIMAGEENC_API IMAGE seuillageImage(IMAGE img, int sInf, int sSup);

extern LIBIMAGEENC_API int* histogrammeImage(IMAGE img, int choix);
extern LIBIMAGEENC_API IMAGE expansionImage(IMAGE img, int outMin, int outMax);
extern LIBIMAGEENC_API IMAGE egalisationImage(IMAGE img);
extern LIBIMAGEENC_API SIGNATURES statistiquesImage(IMAGE img);
extern LIBIMAGEENC_API IMAGERGB colorisationImage(IMAGE img, char *table);

extern LIBIMAGEENC_API IMAGE seuillageOtsu(IMAGE img);

extern LIBIMAGEENC_API IMAGE labelImage(IMAGE img, int *nbComp); /* image label + nb composantes connexes */

extern LIBIMAGEENC_API SIGNATURE_COMPOSANTE_CONNEXE* proprietes(IMAGE label, int nbComp, int voisinage);

extern LIBIMAGEENC_API IMAGE erosionImage(IMAGE img, int voisinage);
extern LIBIMAGEENC_API IMAGE dilatationImage(IMAGE img, int voisinage);
extern LIBIMAGEENC_API IMAGE ouvertureImage(IMAGE img, int voisinage);
extern LIBIMAGEENC_API IMAGE fermetureImage(IMAGE img, int voisinage);

#ifdef __cplusplus
}
#endif