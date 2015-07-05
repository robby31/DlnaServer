#include "decryptyoutubesignature.h"

QHash<QString, QJSEngine*> DecryptYoutubeSignature::cacheJsEngine;
QHash<QString, QString> DecryptYoutubeSignature::cacheJsFun;

DecryptYoutubeSignature::DecryptYoutubeSignature(QNetworkAccessManager *nam, QString youtubeUrl, QString encryptedSignature, QUrl urlForDecoding, QObject *parent) :
    QObject(parent),
    nam(nam),
    m_youtubeUrl(youtubeUrl),
    m_encryptedSignature(encryptedSignature),
    m_urlForDecoding(urlForDecoding),
    m_playerId(),
    m_playerType()
{
    QRegularExpression id_m(".*?-([a-zA-Z0-9_-]+)(?:/watch_as3|/html5player)?\\.([a-z]+)$");
    QRegularExpressionMatch match = id_m.match(urlForDecoding.toString());
    if (match.hasMatch())
    {
        m_playerId = match.captured(1);
        m_playerType = match.captured(2);
    }
    else
    {
        qWarning() << "Unable to evaluate player type";
        qWarning() << urlForDecoding.toString();
    }
}

void DecryptYoutubeSignature::sslErrorsRaised(QList<QSslError> errors)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    reply->ignoreSslErrors();
}

void DecryptYoutubeSignature::decrypt()
{
    if (nam == 0)
    {
        emit error(QString("ERROR, unable to decrypt signature, Network Manager is not initialized."));
    }
    else if (m_playerType == "js")
    {
        if (cacheJsEngine.contains(m_playerId) && cacheJsFun.contains(m_playerId))
        {
            QJSValueList args;
            args << m_encryptedSignature;

            QJSValue fun = cacheJsEngine[m_playerId]->evaluate(cacheJsFun[m_playerId]);
            QJSValue result = fun.call(args);

            if (result.isError())
            {
                // unable to decrypt signature
                emit error(QString("Unable to decrypt signature"));
            }
            else
            {
                QString decrypted_sig = result.toString();
                emit decryptedSignature(m_youtubeUrl + "&signature=" + decrypted_sig);
            }
        }
        else
        {
            QNetworkRequest request(m_urlForDecoding);

            QNetworkReply *reply = nam->get(request);
            connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrorsRaised(QList<QSslError>)));
            connect(reply, SIGNAL(finished()), this, SLOT(decryptSignature()));
        }
    }
    else
    {
        emit error(QString("ERROR, invalid player type %1").arg(m_playerType));
    }
}

void DecryptYoutubeSignature::decryptSignature()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError)
    {
        emit error(reply->errorString());
    }
    else
    {
        QString webpage(reply->readAll());

        cacheJsEngine[m_playerId] = new QJSEngine();
        QJSEngine *jsEngine = cacheJsEngine[m_playerId];

        QRegularExpression funcExp("\\.sig\\|\\|([a-zA-Z0-9$]+)\\(");
        QRegularExpressionMatch funcExpMatch = funcExp.match(webpage);
        if (funcExpMatch.hasMatch())
        {
            QString funcName(funcExpMatch.captured(1));

            QRegularExpression extractFuncExp(QString("(?:function\\s+%1|[{;]%1\\s*=\\s*function)\\s*"
                                                      "\\(([^)]*)\\)\\s*"
                                                      "\\{([^}]+)\\}").arg(funcName));
            QRegularExpressionMatch extractFuncExpMach = extractFuncExp.match(webpage);

            if (extractFuncExpMach.hasMatch())
            {
                QString args_str = extractFuncExpMach.captured(1);
                QString code = extractFuncExpMach.captured(2);

                QJSValue fun = jsEngine->evaluate(QString("(function(%1) { %2; })").arg(args_str).arg(code));
                cacheJsFun[m_playerId] = QString("(function(%1) { %2; })").arg(args_str).arg(code);
                QJSValueList args;
                args << m_encryptedSignature;
                QJSValue result = fun.call(args);
                if (result.isError())
                {
                    QRegularExpression errorExp("ReferenceError: (\\w+) is not defined");
                    QRegularExpressionMatch errorEcpMatch = errorExp.match(result.toString());
                    if (errorEcpMatch.hasMatch())
                    {
                        QString errorVarName(errorEcpMatch.captured(1));

                        QRegularExpression extractVarExp(QString("var\\s+%1\\s*=\\s*"
                                                                 "\\{(.+?)\\};").arg(errorVarName));
                        QRegularExpressionMatch extractVarExpMatch = extractVarExp.match(webpage);
                        if (extractVarExpMatch.hasMatch())
                        {
                            result = jsEngine->evaluate(extractVarExpMatch.captured(0));
                            if (!result.isError())
                                result = fun.call(args);
                        }
                    }
                }

                if (result.isError())
                {
                    cacheJsEngine.remove(m_playerId);
                    cacheJsFun.remove(m_playerId);

                    // unable to decrypt signature
                    emit error(QString("Unable to decrypt signature"));
                }
                else
                {
                    QString decrypted_sig = result.toString();
                    emit decryptedSignature(m_youtubeUrl + "&signature=" + decrypted_sig);
                }
            }
            else
            {
                cacheJsEngine.remove(m_playerId);
                cacheJsFun.remove(m_playerId);

                emit error(QString("Unable to find code for function to decrypt signature"));
            }
        }
        else
        {
            cacheJsEngine.remove(m_playerId);
            cacheJsFun.remove(m_playerId);

            emit error(QString("Unable to locate function to decrypt signature"));
        }
    }

    reply->deleteLater();
}
