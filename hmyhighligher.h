#ifndef HMYHIGHLIGHER_H
#define HMYHIGHLIGHER_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class HMyHighligher : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  HMyHighligher(QTextDocument *parent = 0);

protected:
  void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
  struct HighlightingRule
  {
      QRegExp pattern;
      QTextCharFormat format;
  };
  QVector<HighlightingRule> highlightingRules;
  QTextCharFormat errorFormat;
  QTextCharFormat recvFormat;
  QTextCharFormat sendFormat;
};


#endif // HMYHIGHLIGHER_H
