#include "growingelidetextedit.h"
#include <QDebug>

GrowingElideTextEdit::GrowingElideTextEdit(QWidget *parent) : QTextEdit(parent)
{
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(updateSize()));
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    showReadOnly();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

void GrowingElideTextEdit::setText(const QString &text){
    QTextEdit::setText(text);
    setAlignment(Qt::AlignCenter);
}

QString GrowingElideTextEdit::getFullText(){
    return m_fullText;
}

void GrowingElideTextEdit::setFullText(const QString &text){
    m_fullText = text;
    elideText();
    showSimpleElideText();
}

QString GrowingElideTextEdit::getSimpleWrapText(){
    return m_simpleWrapText;
}

QString GrowingElideTextEdit::getFullWrapText(){
    return m_fullWrapText;
}

QStringList GrowingElideTextEdit::getTexts(){
    return m_texts;
}

bool GrowingElideTextEdit::isSimpleWrapMode(){
    return m_simpleWrapMode;
}

void GrowingElideTextEdit::elideText(){
    QStringList texts;
    QString fullWrapText;
    QString simpleWrapText;
    QFontMetrics fm = this->fontMetrics();
    int fmWidth = width() - 10;
    qDebug() << fmWidth << "============";
    int i = 0, start = 0;

    if (fm.width(m_fullText) < fmWidth){
        texts.append(m_fullText);
    }else{
        for(i = 0; i < m_fullText.length(); i++){
            QString unitString = m_fullText.mid(start, i-start);
            if (fm.width(unitString) < fmWidth){
                continue;
            }else{
                QString retString = m_fullText.mid(start, i - start - 1);
                texts.append(retString);
                start = i -1;
            }
        }
        QString lastString = m_fullText.mid(start, m_fullText.length());
        texts.append(lastString);
    }
    fullWrapText = texts.join("\n");
    if (texts.length() >= 2){
        QString elidedText = fm.elidedText(	QStringList(texts.mid(1)).join("\n"), Qt::ElideRight, fmWidth);
        QStringList simpleList;
        simpleList << texts.at(0) << elidedText;
        simpleWrapText = simpleList.join("\n");
    }else{
        simpleWrapText = texts.at(0);
    }
    m_texts.clear();
    m_texts = texts;
    m_fullWrapText = fullWrapText;
    m_simpleWrapText = simpleWrapText;
}

void GrowingElideTextEdit::showSimpleElideText(){
    qDebug() << "showSimpleElideText" << m_simpleWrapText << m_texts << m_texts.length();
    if (m_texts.length() <= 2){
        setText(m_fullText);
    }else if (m_texts.length() > 2){
        setText(m_simpleWrapText);
    }
    m_simpleWrapMode = true;
}

void GrowingElideTextEdit::showFullWrapText(){
    setText(m_fullText);
    m_simpleWrapMode = false;
}

void GrowingElideTextEdit::updateSize(){
    int docHeight = document()->size().height();
    if (docHeight > m_minHeight && docHeight <= m_maxHeight){
       emit heightChanged(docHeight);
    }
}

void GrowingElideTextEdit::tryRenamed(){
    if (toPlainText().length() == 0){
        showText();
    }else{
        if (toPlainText() != m_fullText){
            emit renameFinished();
        }else{
            showText();
        }
    }
}

void GrowingElideTextEdit::showText(){
    showReadOnly();
    if (m_simpleWrapMode){
        showSimpleElideText();
    }else{
        showFullWrapText();
    }
}

void GrowingElideTextEdit::showReadOnly(){
    setReadOnly(true);
    setTextInteractionFlags(Qt::NoTextInteraction);
    setStyleSheet("\
                  background-color:rgba(0, 0, 0, 0);\
                  border:1px solid rgba(0, 0, 0, 0);\
                  color: white;\
              ");
}

void GrowingElideTextEdit::showEditing(){
    setReadOnly(false);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    selectAll();
    setStyleSheet("\
                  background-color:rgba(0, 0, 0, 0);\
                  border:1px solid black;\
                  color: white;\
              ");
}


void GrowingElideTextEdit::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Escape){
        showText();
        return;
    }else if ((event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Return)
              || (event->modifiers() == Qt::KeypadModifier && event->key() == Qt::Key_Enter)){
        qDebug() << "Key_Return" << "Key_Enter";
        tryRenamed();
        return;
    }
    QTextEdit::keyPressEvent(event);
}

//void GrowingElideTextEdit::enterEvent(QEvent *event){
//    qDebug() << event <<"======";
//    if (isReadOnly()){
//        clearFocus();
//        unsetCursor();
//    }
//    QTextEdit::enterEvent(event);
//}

GrowingElideTextEdit::~GrowingElideTextEdit()
{

}
