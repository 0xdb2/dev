// $Id$
// loadjava -u db@mpsrv.local.net:1521:myoradb -v -r -t core/log/MyLog.java
// dropjava -u db/myoradb\$db@mpsrv.local.net:1521:myoradb -v core/log/MyLog
/*

*/

package core.log;

import java.util.Date;
import java.util.logging.*;
import java.text.SimpleDateFormat;
import java.io.IOException;

public class myLogger
{
    private static Logger log;
    public static Logger getLogger() { return log; }

    public static void init(String logfile, String name) throws IOException
    {
        //Logger log = Logger.getLogger(name);
        log = Logger.getLogger(name);

        Handler fileHandler = new FileHandler(logfile, 1024, 10, false);
        //SimpleFormatter formatter = new SimpleFormatter();
        myFormatter formatter = new myFormatter();
        fileHandler.setFormatter(formatter);
        log.addHandler(fileHandler);
        fileHandler.setLevel(Level.ALL);
        log.setLevel(Level.ALL);
        ConsoleHandler consoleHandler = new ConsoleHandler();
        consoleHandler.setFormatter(formatter);
        consoleHandler.setLevel(Level.ALL);
        log.addHandler(consoleHandler);
        log.info("Log file " + logfile + " opened successfully");
        log.setUseParentHandlers(false);
        //return log;
    }
}

class myFormatter extends Formatter
{
    @Override
    public String format(LogRecord record)
    {
        //10::orderman::addOrder::2015-02-06::addOrder: pstmt=oracle.jdbc.driver.OraclePreparedStatementWrapper@976360ef
        //+ new Date(record.getMillis()) + "::"
        //    + record.getThreadID() + "::" + record.getSourceClassName() + "::" + record.getSourceMethodName() + "::"
        return new SimpleDateFormat("yyyy.MM.dd HH:mm:ss.SSS").format(new Date(record.getMillis())) + " - "
            + record.getSourceClassName() + "::" + record.getSourceMethodName() + ":"
            + record.getMessage() + "\n"
            ;
    }
}

