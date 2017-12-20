// $Id$
// loadjava -u db@mpsrv.local.net:1521:myoradb -v -r -t orderman.java
/*

*/

import java.sql.*;
import java.io.*;
import oracle.jdbc.*;

import java.util.logging.Logger;
//import java.util.logging.FileHandler;

import core.log.myLogger;
import java.io.IOException;

public class orderman
{
    private static String my = "init", tmp;
    //private static final Logger log = myLogger.getFileLogger("/home/db/log/ora/orderman.log", orderman.class.getName());
    private static Logger log; //  = Logger.getLogger(orderman.class.getName());

    public static void init(String logfile) throws IOException
    {
        myLogger.init(logfile, orderman.class.getName());
        log = myLogger.getLogger();
    }

    public static void addCustomer (int custNo, String custName, String street,
            String city, String state, String zipCode, String phoneNo) throws SQLException
    {
        String sql = "INSERT INTO Customers VALUES (?,?,?,?,?,?,?)";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, custNo);
            pstmt.setString(2, custName);
            pstmt.setString(3, street);
            pstmt.setString(4, city);
            pstmt.setString(5, state);
            pstmt.setString(6, zipCode);
            pstmt.setString(7, phoneNo);
            pstmt.executeUpdate();
            pstmt.close();
        }
        catch (SQLException e)
        {

            System.err.println("addCustomer: " + e.getMessage());
        }
        tmp = my;
        my = "addCustomer";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void addStockItem (int stockNo, String description, float price)
        throws SQLException
    {
        String sql = "INSERT INTO StockItems VALUES (?,?,?)";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, stockNo);
            pstmt.setString(2, description);
            pstmt.setFloat(3, price);
            pstmt.executeUpdate();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println("addStockItem: " + e.getMessage());
        }
        tmp = my;
        my = "addStockItem";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void addOrder (int orderNo, int custNo, String orderDate,
            String shipDate, String toStreet, String toCity, String toState,
            String toZipCode) throws SQLException
    {
        String sql = "INSERT INTO Orders VALUES (?,?,?,?,?,?,?,?)";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, orderNo);
            pstmt.setInt(2, custNo);
            pstmt.setString(3, orderDate);
            pstmt.setString(4, shipDate);
            pstmt.setString(5, toStreet);
            pstmt.setString(6, toCity);
            pstmt.setString(7, toState);
            pstmt.setString(8, toZipCode);
            log.info("addOrder: pstmt=" + pstmt);
            pstmt.executeUpdate();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println("addOrder: " + e.getMessage());
        }
        tmp = my;
        my = "addOrder";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void addLineItem (int lineNo, int orderNo, int stockNo,
            int quantity, float discount) throws SQLException
    {
        String sql = "INSERT INTO LineItems VALUES (?,?,?,?,?)";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, lineNo);
            pstmt.setInt(2, orderNo);
            pstmt.setInt(3, stockNo);
            pstmt.setInt(4, quantity);
            pstmt.setFloat(5, discount);
            pstmt.executeUpdate();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println("addLineItem: " + e.getMessage());
        }
        tmp = my;
        my = "addLineItem";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void totalOrders () throws SQLException
    {
        String sql = "SELECT O.orderNo, ROUND(SUM(S.Price * L.Quantity)) AS TOTAL " +
            "FROM Orders O, LineItems L, StockItems S " +
            "WHERE O.orderNo = L.orderNo AND L.StockNo = S.StockNo " +
            "GROUP BY O.orderNo";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            ResultSet rset = pstmt.executeQuery();
            printResults(rset);
            rset.close();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println("totalOrders: " + e.getMessage());
        }
        tmp = my;
        my = "totalOrders";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    static void printResults (ResultSet rset) throws SQLException
    {
        String buffer = "";
        try
        {
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
            if (rows == 0)
                buffer = "No data found!\n";
            System.out.println(buffer);
        }
        catch (SQLException e)
        {
            System.err.println("printResults: " + e.getMessage());
        }
        tmp = my;
        my = "printResults";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void checkStockItem (int stockNo) throws SQLException
    {
        String sql = "SELECT O.orderNo, O.CustNo, L.StockNo, " +
            "L.LineNo, L.Quantity, L.Discount " +
            "FROM Orders O, LineItems L " +
            "WHERE O.orderNo = L.orderNo AND L.StockNo = ?";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, stockNo);
            ResultSet rset = pstmt.executeQuery();
            printResults(rset);
            rset.close();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println(e.getMessage());
        }
        tmp = my;
        my = "checkStockItem";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void changeQuantity (int newQty, int orderNo, int stockNo)
        throws SQLException
    {
        String sql = "UPDATE LineItems SET Quantity = ? " +
            "WHERE orderNo = ? AND StockNo = ?";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, newQty);
            pstmt.setInt(2, orderNo);
            pstmt.setInt(3, stockNo);
            pstmt.executeUpdate();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println(e.getMessage());
        }
        tmp = my;
        my = "changeQuantity";
        System.out.println("my=" + my + '(' + tmp + ')');
    }

    public static void deleteOrder (int orderNo) throws SQLException
    {
        String sql = "DELETE FROM LineItems WHERE orderNo = ?";
        try
        {
            Connection conn = DriverManager.getConnection("jdbc:default:connection:");
            PreparedStatement pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, orderNo);
            pstmt.executeUpdate();
            sql = "DELETE FROM Orders WHERE orderNo = ?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, orderNo);
            pstmt.executeUpdate();
            pstmt.close();
        }
        catch (SQLException e)
        {
            System.err.println(e.getMessage());
        }
        tmp = my;
        my = "deleteOrder";
        System.out.println("my=" + my + '(' + tmp + ')');
    }
}


