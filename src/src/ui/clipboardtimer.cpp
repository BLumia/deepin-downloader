/**
 * @copyright 2020-2020 Uniontech Technology Co., Ltd.
 *
 * @file clipboardtimer.h
 *
 * @brief 剪切板托管类
 *
 * @date 2020-06-09 10:51
 *
 * Author: bulongwei  <bulongwei@uniontech.com>
 *
 * Maintainer: bulongwei  <bulongwei@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "clipboardtimer.h"
#include <QClipboard>


ClipboardTimer::ClipboardTimer(QObject *parent) :
    QObject(parent)
{
    m_clipboard = QApplication::clipboard();        //获取当前剪切板
    connect(m_clipboard,&QClipboard::dataChanged,this,&ClipboardTimer::getDataChanged);

}

ClipboardTimer::~ClipboardTimer()
{

}


void ClipboardTimer::getDataChanged()
{
    QString _url = m_clipboard->text();
    Settings *_setting =  Settings::getInstance();
    bool _bIsHttp =  _setting->getHttpDownloadState();
    bool _bIsMagnet = _setting->getMagneticDownloadState();
    bool _bIsBt = _setting->getBtDownloadState();
    if((isMagnet(_url) && _bIsMagnet) ||
            (isHttp(_url) && _bIsHttp))
    {
        emit sendClipboardText(_url);
    }
    if(_url.endsWith(".torrent") && _bIsBt)
    {
        m_clipboard->clear();
        emit sentBtText(_url);
    }
}

bool ClipboardTimer::isMagnet(QString url)
{
    QString _str = url;
    if(_str.mid(0,20) == "magnet:?xt=urn:btih:")
    {
        return  true;
    }
    else
    {
        return  false;
    }
}

bool ClipboardTimer::isHttp(QString url)
{
    if( (-1 == url.indexOf("ftp:")) && (-1 == url.indexOf("http://")) && (-1 == url.indexOf("https://")))
    {
        return false;
    }
    QStringList _list= url.split(".");
    QString _suffix = _list[_list.size()-1];
    QStringList _type;
     _type<< "asf"<<"avi"<<"iso"<<"mp3"<<"mpeg"<<"ra"<<"rar"<<"rm"<<"rmvb"<<"tar"<<"wma"<<"wmp"<<"wmv"<<"mov"<<"zip"<<"3gp"<<"chm"<<"mdf"<<"torrent"<<"jar"<<"msi"<<"arj"<<"bin"<<"dll"<<"psd"<<"hqx"<<"sit"<<"lzh"<<"gz"<<"tgz"<<"xlsx"<<"xls"<<"doc"<<"docx"<<"ppt"<<"pptx"<<"flv"<<"swf"<<"mkv"<<"tp"<<"ts"<<"flac"<<"ape"<<"wav"<<"aac"<<"txt"<<"dat"<<"7z"<<"ttf"<<"bat"<<"xv"<<"xvx"<<"pdf"<<"mp4"<<"apk"<<"ipa"<<"epub"<<"mobi"<<"deb"<<"sisx"<<"cab"<<"pxl"<<"xlb"<<"dmg"<<"msu"<<"bz2";
    if(_type.contains(_suffix))
    {
        return true;
    }
    for (int i = 0; i < _type.size(); i++) {
        if(_type[i].toUpper() == _suffix)
        {
            return true;
        }
    }
    return false;
}
