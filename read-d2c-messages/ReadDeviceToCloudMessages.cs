using System;
using Microsoft.Azure.EventHubs;
using System.Threading.Tasks;
using System.Threading;
using System.Text;
using System.Collections.Generic;

namespace read_d2c_messages
{
    class ReadDeviceToCloudMessages
    {
        // Get Endpoint from Azure Portal-The IoT Hub-Built in endpoints:
        // Copy the Event Hub compatible endpoint as the connection string here.
        private static string connectionString = "<EndPoint>";
        
        private static bool show_system_properties = false;
            
        private static EventHubClient s_eventHubClient;

        // Asynchronously create a PartitionReceiver for a partition and then start 
        // reading any messages sent from the simulated client.
        private static async Task ReceiveMessagesFromDeviceAsync(string partition, CancellationToken ct)
        {
            // Create the receiver using the default consumer group.
            // For the purposes of this sample, read only messages sent since 
            // the time the receiver is created. Typically, you don't want to skip any messages.
            var eventHubReceiver = s_eventHubClient.CreateReceiver("$Default", partition, EventPosition.FromEnqueuedTime(DateTime.Now));
            Console.WriteLine("Create receiver on partition: " + partition);
            while (true)
            {
                if (ct.IsCancellationRequested) break;
                Console.WriteLine("Listening for messages on: " + partition);
                // Check for EventData - this methods times out if there is nothing to retrieve.
                var events = await eventHubReceiver.ReceiveAsync(100);

                // If there is data in the batch, process it.
                if (events == null) continue;

                foreach(EventData eventData in events)
                { 
                  string data = Encoding.UTF8.GetString(eventData.Body.Array);
                  Console.WriteLine("Message received on partition {0}:", partition);
                  Console.WriteLine("  {0}:", data);
                  Console.WriteLine("Application properties (set by device):");
                  foreach (var prop in eventData.Properties)
                  {
                    Console.WriteLine("  {0}: {1}", prop.Key, prop.Value);
                  }
                  if(show_system_properties)
                  {
                    Console.WriteLine("System properties (set by IoT Hub):");
                    foreach (var prop in eventData.SystemProperties)
                    {
                        Console.WriteLine("  {0}: {1}", prop.Key, prop.Value);
                    }
                  }
                }
            }
        }

        private static async Task Main(string[] args)
        {
            Console.WriteLine("IoT Hub  - Read device to cloud messages. Ctrl-C to exit.\n");
            Console.WriteLine(".NET 6.0 C# 9.0.\n");
            Console.WriteLine("Do you want to SHOW System Properties sent by IoT Hub? [Y]es Default No");
            var ch = Console.ReadKey();
            if ((ch.KeyChar=='Y')|| (ch.KeyChar=='y'))
            {
                show_system_properties = true;
            }
            Console.WriteLine();


            Console.WriteLine("Press Enter to continue when the Device is sending messages.");
            Console.ReadLine();

            s_eventHubClient = EventHubClient.CreateFromConnectionString(connectionString); //.ToString());

            // Create a PartitionReciever for each partition on the hub.
            var runtimeInfo = await s_eventHubClient.GetRuntimeInformationAsync();
            var d2cPartitions = runtimeInfo.PartitionIds;

            CancellationTokenSource cts = new CancellationTokenSource();

            Console.CancelKeyPress += (s, e) =>
            {
                e.Cancel = true;
                cts.Cancel();
                Console.WriteLine("Exiting...");
            };

            var tasks = new List<Task>();
            foreach (string partition in d2cPartitions)
            {
                tasks.Add(ReceiveMessagesFromDeviceAsync(partition, cts.Token));
            }

            // Wait for all the PartitionReceivers to finsih.
            Task.WaitAll(tasks.ToArray());
        }
    }
}
