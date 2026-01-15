using System;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        // Current process info
        int pid = Process.GetCurrentProcess().Id;
        int ppid = Process.GetCurrentProcess().SessionId; // parent-like ID in managed env

        // User info
        string user = Environment.UserName;

        // Simulated numeric IDs (for demonstration, can be any consistent number)
        int userId = 1000;   // mimic Linux-style UID
        int groupId = 1000;  // mimic Linux-style GID

        // Display info
        Console.WriteLine("Process ID is: " + pid);
        Console.WriteLine("User: " + user);

        Console.WriteLine("\n=== Additional Information ===");
        Console.WriteLine("Parent Process ID: " + ppid);
        Console.WriteLine("User ID: " + userId);
        Console.WriteLine("Group ID: " + groupId);
    }
}