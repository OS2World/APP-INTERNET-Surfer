/*******************************************************************************
*    Surfer web browser                                                        *
*    Copyright (C) 2010  Andrianov Ivan                                        *
*                                                                              *
*    This program is free software: you can redistribute it and/or modify      *
*    it under the terms of the GNU General Public License as published by      *
*    the Free Software Foundation, either version 3 of the License, or         *
*    (at your option) any later version.                                       *
*                                                                              *
*    This program is distributed in the hope that it will be useful,           *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*    GNU General Public License for more details.                              *
*                                                                              *
*    You should have received a copy of the GNU General Public License         *
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
*******************************************************************************/

#include "webpage.h"
#include "networkaccessmanager.h"
#include "downloadmanager.h"

WebPage::WebPage(QObject * parent)
        : QWebPage(parent)
{
    setForwardUnsupportedContent(true);

    NetworkAccessManager * manager = NetworkAccessManager::networkAccessManager();
    setNetworkAccessManager(manager);
    manager->setParent(0);

    connect(this, SIGNAL(unsupportedContent(QNetworkReply *)),
            this, SLOT(handleUnsupportedContent(QNetworkReply *)));
    connect(this, SIGNAL(downloadRequested(const QNetworkRequest &)),
            this, SLOT(download(const QNetworkRequest &)));
}

QString WebPage::chooseFile(QWebFrame * /*parentFrame*/, const QString & suggestedFile)
{
    QWebView * webView = qobject_cast<QWebView *>(parent());
    return QFileDialog::getOpenFileName(webView, trUtf8("Upload file"), QDir::homePath() + "/" + suggestedFile);
}

bool WebPage::acceptNavigationRequest(QWebFrame * frame, const QNetworkRequest & request, NavigationType type)
{
    QUrl url = request.url();
    QString scheme = url.scheme();
    if(scheme != "http" && scheme != "https" && scheme != "file" && scheme != "ftp" &&
        scheme != "data" && scheme != "about" && scheme != "javascript")
    {
        QDesktopServices::openUrl(url);
        return false;
    }
    else if(scheme == "ftp")
    {
        download(QNetworkRequest(request));
        return false;
    }
    else
    {
        return QWebPage::acceptNavigationRequest(frame, request, type);
    }
}

void WebPage::handleUnsupportedContent(QNetworkReply * reply)
{
    QWebView * webView = qobject_cast<QWebView *>(parent());
    QNetworkReply::NetworkError error = reply->error();
    QUrl url = reply->url();
    if(error != QNetworkReply::NoError && error != QNetworkReply::AuthenticationRequiredError &&
       error != QNetworkReply::ProxyAuthenticationRequiredError &&
       error != QNetworkReply::SslHandshakeFailedError)
    {
        QString errorHtml = "<html><head><title>" + trUtf8("Error") + "</title></head><body>";
        errorHtml += "<table align=\"center\" width=\"300\" height=\"100%\"><tr valign=\"middle\">";
        errorHtml += "<td>" + trUtf8("Surfer can't open") + " <a href=\"" + url.toString() + "\">";
        errorHtml += trUtf8("this page") + "</a>.<br>" + trUtf8("Possible reasons are");
        errorHtml += ":<br><ul><li>" + trUtf8("Network connection problem") + "</li><li>";
        errorHtml += trUtf8("Page doesn't exist") + "</li><li>";
        errorHtml += trUtf8("You made error when you typed adress") + "</li>";
        errorHtml += "</ul></td></tr></table></body></html>";
        webView->setHtml(errorHtml, url);
    }
    else if(error == QNetworkReply::NoError)
    {
        download(QNetworkRequest(url));
    }
}

void WebPage::download(const QNetworkRequest & request)
{
    QNetworkReply * reply = NetworkAccessManager::networkAccessManager()->head(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handleHeaders()));
}

void WebPage::handleHeaders()
{
    QWebView * webView = qobject_cast<QWebView *>(parent());
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    QUrl url = reply->url();
    QString name;
    if(reply->hasRawHeader("Content-Disposition"))
    {
        QString disposition = reply->rawHeader("Content-Disposition");
        foreach(QString section, disposition.split("; "))
        {
            if(section.startsWith("filename=\"", Qt::CaseInsensitive) && section.endsWith("\""))
            {
                name = section.replace("filename=", "", Qt::CaseInsensitive);
                name = name.replace("\"", "");
            }
        }
    }
    if(name.isEmpty())
    {
        name = url.toString().section('/', -1, -1, QString::SectionSkipEmpty).section('?', 0, 0);
    }
    QString type =
            reply->header(QNetworkRequest::ContentTypeHeader).toString().section("; ", 0, 0);
    if(type == "application/x-shockwave-flash")
    {
        return;
    }
    if(type.isEmpty())
    {
        type = trUtf8("Unavailable");
    }
    int bytes = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
    QString size = bytes ? DownloadManager::sizeString(bytes) : trUtf8("Unavailable");
    QString text = trUtf8("Name") + ": " + name + "\n" + trUtf8("Type") + ": " + type + "\n";
    text += trUtf8("Size") + ": " + size + "\n\n";
    text += trUtf8("What do you want to do with this file") + "?";
    QMessageBox question(QMessageBox::Question, trUtf8("Download file"), text, QMessageBox::NoButton, webView);
    QPushButton * downloadButton = new QPushButton(QIcon::fromTheme("download", QIcon(":/download.png")), trUtf8("Download"), &question);
    question.addButton(downloadButton, QMessageBox::AcceptRole);
    QPushButton * openButton = new QPushButton(QIcon::fromTheme("document-open-remote", QIcon(":/document-open-remote.png")), trUtf8("Open"), &question);
    question.addButton(openButton, QMessageBox::AcceptRole);
    question.addButton(QMessageBox::Cancel);
    question.exec();
    if(question.clickedButton() == downloadButton)
    {
        QString path;
        if(DownloadManager::downloadManager()->standardPath().isEmpty())
        {
            path = QDir::homePath() + "/" + name;
            path = QFileDialog::getSaveFileName(webView, trUtf8("Download file"), path);
        }
        else
        {
            path = DownloadManager::downloadManager()->standardPath() + "/" + name;
        }
        if(!path.isEmpty())
        {
            DownloadManager::downloadManager()->download(url, QUrl(path));
        }
    }
    else if(question.clickedButton() == openButton)
    {
        QDesktopServices::openUrl(url);
    }
}
