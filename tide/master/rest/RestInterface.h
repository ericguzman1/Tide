/*********************************************************************/
/* Copyright (c) 2016, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of Ecole polytechnique federale de Lausanne.          */
/*********************************************************************/

#ifndef RESTINTERFACE_H
#define RESTINTERFACE_H

#include "types.h"
#include "JsonSize.h"
#include "RestLogger.h"

#include <QObject>

/**
 * Enables remote control of Tide through a REST API.
 *
 * It listens for http PUT requests on 'http://hostname:port/tide/\<command\>'
 * and emits the corresponding \<command\> signal on success.
 *
 * Example command:
 * curl -i -X PUT -d '{"uri": "image.png"}' http://localhost:8888/tide/open
 *
 * It also exposes a control html interface on 'http://hostname:port'.
 */
class RestInterface : public QObject
{
    Q_OBJECT

public:
    /**
     * Construct a REST interface.
     * @param port the port for listening to REST requests
     * @param options the application's options to expose in the interface
     * @param config the application's configuration
     * @throw std::runtime_error if the port is already in use or a connection
     *        issue occured.
     */
    RestInterface( int port, OptionsPtr options,
                   const MasterConfiguration& config );

    /** Out-of-line destructor. */
    ~RestInterface();

    /** Expose the statistics gathered by the given logging utility. */
    void exposeStatistics( const LoggingUtility& logger ) const;

    /**
     * Set-up the HTML interface.
     * @param displayGroup DisplayGroup exposed via interface
     * @param config MasterConfiguration used to set-up the interface
     */
    void setupHtmlInterface( DisplayGroup& displayGroup,
                             const MasterConfiguration& config );

signals:
    /** Open a content. */
    void open( QString uri );

    /** Load a session. */
    void load( QString uri );

    /** Save a session to the given file. */
    void save( QString uri );

    /** Clear all contents. */
    void clear();

    /** Open a whiteboard. */
    void whiteboard();

    /** Close a content. */
    void close( QString uuid );

    /** Browse a website. */
    void browse( QString uri );

    /** Take a screenshot. */
    void screenshot( QString uri );

    /** Exit the application. */
    void exit();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
    const MasterConfiguration& _config;
};

#endif
