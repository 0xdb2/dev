class Args
{
    public static void main(String[] args)
    {
        if (args.length == 0)
        {
            System.err.println("\nUsage: args arg1 [arg2 ...]");
            System.exit(1);
        }
        else
        {
            System.out.print("\nargs=");
            for (String s:args)
            {
                System.out.print(s + " ");

            }
            System.out.print("\n");
        }
        System.exit(0);
    }
}
