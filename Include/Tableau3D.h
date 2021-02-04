#pragma once
#include <vector>

template <typename T>
class CTableau3D
{
	size_t m_sizeHauteur;
	size_t m_sizeProfondeur;
	std::vector<T> data;

public:
	CTableau3D();
	CTableau3D(size_t largeur, size_t hauteur, size_t profondeur);
	CTableau3D(size_t largeur, size_t hauteur, size_t profondeur, T initValue);

	T& at(int x, int y, int z);

	T at(int x, int y, int z) const;
};

template<typename T>
inline CTableau3D<T>::CTableau3D() : m_sizeHauteur(0), m_sizeProfondeur(0)
{
}

template<typename T>
inline CTableau3D<T>::CTableau3D(size_t largeur, size_t hauteur, size_t profondeur) : m_sizeHauteur(hauteur), m_sizeProfondeur(profondeur)
{ 
	data.resize(largeur * hauteur * profondeur);
}

template<typename T>
inline CTableau3D<T>::CTableau3D(size_t largeur, size_t hauteur, size_t profondeur, T initValue) : m_sizeHauteur(hauteur), m_sizeProfondeur(profondeur)
{
	data.assign(largeur * hauteur * profondeur, initValue);
}

template<typename T>
inline T & CTableau3D<T>::at(int x, int y, int z)
{
	return data[x * m_sizeHauteur * m_sizeProfondeur + y * m_sizeProfondeur + z];
}

template<typename T>
inline T CTableau3D<T>::at(int x, int y, int z) const
{
	return data[x * m_sizeHauteur * m_sizeProfondeur + y * m_sizeProfondeur + z];
}

