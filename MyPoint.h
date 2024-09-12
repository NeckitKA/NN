//---------------------------------------------------------------------------

#ifndef MyPointH
#define MyPointH
//---------------------------------------------------------------------------
class MyPoint{
	private:
		int X, Y;
		bool Color;
		void SetX(int x);
		void SetY(int y);
		void SetColor(bool color);

	public:
		MyPoint(int x, int y, bool color);

		int GetX() const;
		int GetY() const;
		bool GetColor() const;
		void ShowPoint();
};
//---------------------------------------------------------------------------
#endif
