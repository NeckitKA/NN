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
		void SetColor(int color);

	public:
		MyPoint(int x, int y, int color);

		int GetX() const;
		int GetY() const;
		int GetColor() const;
		void ShowPoint();
};
//---------------------------------------------------------------------------
#endif
