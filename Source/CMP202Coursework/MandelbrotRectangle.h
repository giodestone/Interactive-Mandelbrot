#pragma once

#include <SFML/Graphics/Rect.hpp>

/// <summary>
/// Special rectangle that keeps its Left, Right, Top, Bottom values.
/// When you want one with transformations applied (scale, translation) call UpdateTranslated(), then GetTranslated().
/// </summary>
template <typename T>
struct MandelbrotRectangle
{
private:
	sf::Rect<T> translated;

public:
	T Left, Right, Top, Bottom;
	T Scale;
	T OriginalScale;
	sf::Vector2<T> Translation;

	MandelbrotRectangle() {}
	MandelbrotRectangle(T left, T right, T top, T bottom, T startScale) :
		Left(left), Right(right), Top(top), Bottom(bottom),
		translated({ left, top }, { right, bottom }),
		Scale(startScale), OriginalScale(startScale)
	{}
	~MandelbrotRectangle() {}

	/// <summary>
	/// Resets the translation and scale to their original values.
	/// </summary>
	void Reset()
	{
		Scale = OriginalScale;
		Translation = sf::Vector2<T>();
	}

	/// <summary>
	/// Updates the rectangle with the translations applied.
	/// </summary>
	/// <returns></returns>
	/// <remarks>Should be used sparignly as it contains 12 operations - i.e. only when a change to the interal paramters has taken place.</remarks>
	void UpdateTranslated()
	{
		translated = sf::Rect<T>({ Left, Top }, { Right, Bottom });

		//Scale
		translated.left *= Scale;
		translated.width *= Scale;
		translated.top *= Scale;
		translated.height *= Scale;

		//Rotate

		//Translate
		translated.left += Translation.x;
		translated.width += Translation.x;
		translated.top += Translation.y;
		translated.height += Translation.y;
	}

	/// <summary>
	/// Retruns the translated rectangle.
	/// </summary>
	/// <returns>The translated rectangle.</returns>
	/// <remarks>Treat Rect::width as right, and Rect::height as bottom.</remarks>
	sf::Rect<T>& GetTranslated()
	{
		return translated;
	}
};