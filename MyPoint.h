//---------------------------------------------------------------------------

#ifndef MyPointH
#define MyPointH
//---------------------------------------------------------------------------
class MyPoint{
	private:
		int X, Y, Color;

		void SetX(double x);
		void SetY(double y);
		void SetColor(int color);

	public:
		MyPoint(double x, double y, int color);

		double GetX() const;
		double GetY() const;
		int GetColor() const;
		void ShowPoint();
};
//---------------------------------------------------------------------------
#endif
