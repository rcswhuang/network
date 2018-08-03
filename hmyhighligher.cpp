#include "hmyhighligher.h"
#include <QFontMetrics>

HMyHighligher::HMyHighligher(QTextDocument *parent)
      : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QFont sansFont("Helvetica [Cronyx]", 12);

    recvFormat.setFontWeight(QFont::Bold);
    recvFormat.setForeground(QColor("#B03060"));
    recvFormat.setFont(sansFont);
    recvFormat.setProperty(QTextFormat::LineHeight,QFontMetrics(sansFont).height()+40);
    rule.pattern = QRegExp("[\u4e00-\u9fa5a-zA-Z0-9_ :<-\(]+<<[a-zA-Z0-9 ]+");
    rule.format = recvFormat;
    highlightingRules.append(rule);


    sendFormat.setFontWeight(QFont::Bold);
    sendFormat.setForeground(Qt::blue);
    sendFormat.setFont(sansFont);
    sendFormat.setProperty(QTextFormat::LineHeight,QFontMetrics(sansFont).height()+40);
    rule.pattern = QRegExp("[\u4e00-\u9fa5a-zA-Z0-9_ :<-\(]+>>[a-zA-Z0-9 ]+");
    rule.format = sendFormat;
    highlightingRules.append(rule);
}

void HMyHighligher::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
              QRegExp expression(rule.pattern);
              int index = expression.indexIn(text);
              while (index >= 0) {
                  int length = expression.matchedLength();
                  setFormat(index, length, rule.format);
                  index = expression.indexIn(text, index + length);
              }
          }
}
