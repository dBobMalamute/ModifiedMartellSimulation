#ifndef MARTELL1996TABLELEVEL_H
#define MARTELL1996TABLELEVEL_H

#include <QObject>

class Martell1996TableLevel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged)
    Q_PROPERTY(double meanScore READ meanScore WRITE setMeanScore NOTIFY meanScoreChanged)
    Q_PROPERTY(int numPositions READ numPositions WRITE setNumPositions NOTIFY numPositionsChanged)
    Q_PROPERTY(double percentWomen READ percentWomen WRITE setPercentWomen NOTIFY percentWomenChanged)

public:
    explicit Martell1996TableLevel(QObject *parent = nullptr);
    Martell1996TableLevel(const int level, const double meanScore,
                          const int numPositions, const double percentWomen, QObject* parent);

    int level() const;
    double meanScore() const;
    int numPositions() const;
    double percentWomen() const;

public slots:
    void setLevel(int level);
    void setMeanScore(double meanScore);
    void setNumPositions(int numPositions);
    void setPercentWomen(double percentWomen);

signals:
    void levelChanged(int level);
    void meanScoreChanged(double meanScore);
    void percentWomenChanged(double percentWomen);
    void numPositionsChanged(int numPositions);

private:
    int m_level;
    double m_meanScore;
    int m_numPositions;
    double m_percentWomen;
};

#endif // MARTELL1996TABLELEVEL_H
