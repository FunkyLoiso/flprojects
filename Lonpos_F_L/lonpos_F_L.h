class FieldPlace : public QPoint
{
public:
	typedef list QList<FieldPlace>
}

class Figure
{
public:
	typedef list QList<Figure>;
	
	Figure(const QString& configuration);
	
	int width() const;
	int height() const;
	FieldPlace::list getPlaces() const;
	FieldPlace getCenter() const;
	void setCenter(FieldPlace place);
	void move(int dx, int dy);
	
	void rotateCW();
	void rotateCCW();
	QColor getColor();
	
private:
	QColor color;
	FieldPlace place;
	FieldPlace::list relativePlaces;
}

class FieldWidget
{
public:
	FieldWidget(const QString& configuration);
	
	void setFigures(Figure::list* figures);
	void setOverlayFigure(FieldPlace place, const Figure&);
	void removeOverlayFigure();
	void setTrackingFigure(const Figure&);
	void removeTrackingFigure();
	
signals:
	void lmbClicked(FieldPlace place);
	void rmbClicked();
	void rotated(bool clockwise);
	void removed();
}

class FieldController
{
public:
public slots:
	void onSideBarLmbClicked(FieldPlace);
	void onSideBarRmbClicked();
	void onFieldLmbClicked(FieldPlace);
	void onFieldRmbClicked();
	void onFieldRotated();
	void onFieldRemoved();
	
private:
	
}

