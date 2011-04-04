

class test1 : public QObject
{
  Q_OBJECT
private slots:
  void testTrivial(void) {
    int i = 3;
    QVERIFY(i == 3);
  }
};
