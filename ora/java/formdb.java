import form.FormBuilder;

import oracle.jdbc.*;
import java.sql.*;

import java.util.logging.Logger;
import core.log.myLogger;
import java.io.IOException;

public class FormDb
{
    private static Logger log; //  = Logger.getLogger(orderman.class.getName());
    private static FormBuilder builder;

    public static void  init(String logfile) throws IOException
    {
        myLogger.init(logfile, FormDb.class.getName());
        log = myLogger.getLogger();

        builder = new FormBuilder();

        builder.loadTemplate("/home/db/dev/ora/java/test_form.tpl");
        log.info("form=" + builder.getForm());

    }

    public static void setDataSet(String sqlStmt)
    {
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sqlStmt);
            ResultSet rset = pstmt.executeQuery();
            builder.loadDataSet(rset);
            /*---------------------------------------------------------- moved to FormBuilder.loadDataSet(ResultSet rs)
            String buffer = "";

            ResultSetMetaData meta = rset.getMetaData();
            int cols = meta.getColumnCount(), rows = 0;
            for (int i = 1; i <= cols; i++)
            {
                int size = meta.getPrecision(i);
                String label = meta.getColumnLabel(i);
                if (label.length() > size)
                    size = label.length();
                while (label.length() < size)
                    label += " ";
                buffer = buffer + label + " ";
            }
            buffer = buffer + "\n";

            while (rset.next())
            {
                rows++;
                for (int i = 1; i <= cols; i++)
                {
                    int size = meta.getPrecision(i);
                    String label = meta.getColumnLabel(i);
                    String value = rset.getString(i);
                    if (label.length() > size)
                        size = label.length();
                    while (value.length() < size)
                        value += " ";
                    buffer = buffer + value + " ";
                }
                buffer = buffer + "\n";
            }
            if (rows == 0)  buffer = "No data found!\n";
            System.out.println(buffer);
            ---------------------------------------------------------- end
            */

            rset.close();
            pstmt.close();
            log.info("resultset ready for form=" + builder.getForm());
        }
        catch (SQLException e)
        {
            System.err.println("getDataSet: " + e.getMessage());
        }
    }

    public static void getOutput()
    {
        builder.getOutput();
    }
}
