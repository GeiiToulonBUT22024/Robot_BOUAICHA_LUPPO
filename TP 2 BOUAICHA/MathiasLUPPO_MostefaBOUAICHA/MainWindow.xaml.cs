using System;
using System.IO.Ports;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using ExtendedSerialPort;
using System.Windows.Threading;


namespace MathiasLUPPO_MostefaBOUAICHA
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //string receivedText;
        bool flag = true;

        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM12", 115200, Parity.None, 8, StopBits.One);
            serialPort1.OnDataReceivedEvent += SerialPort1_DataReceived;
            serialPort1.Open();
            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

        }

        public void TimerAffichage_Tick(object sender, EventArgs e)
        {

            //byte[] tab = new byte[robot.byteListReceived.Count];
            //int idx = 0;
            //while (robot.byteListReceived.Count > 0)
            //{
            //    tab[idx] = robot.byteListReceived.Dequeue();
            //    idx++;
            //}

            //textBox_Reception.Text += Encoding.UTF8.GetString(tab);

            while (robot.byteListReceived.Count > 0)
            {
                byte b = robot.byteListReceived.Dequeue();
                DecodeMessage(b);
            } 
        }

        public void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            for (int i = 0; i < e.Data.Length; i++)
            {
                robot.byteListReceived.Enqueue(e.Data[i]);
            }
        }


        #region Button 
        private void BoutonEnvoyerClick(object sender, RoutedEventArgs e)
        {
            sendMessage();
            textBox_Emission.Text = "";

            if (flag == true)
            {
                BoutonEnvoyer.Background = Brushes.RoyalBlue;
                flag = !flag;
            }
            else if (flag == false)
            {
                BoutonEnvoyer.Background = Brushes.Black;
                flag = !flag;
            }

        }
        public void BoutonTESTClick(object sender, RoutedEventArgs e)
        {
            byte[] byteListToSend = new byte[20 + 2 + 2 + 1 + 1];
            byteListToSend[0] = 0xFE;
            byteListToSend[1] = 0x00;
            byteListToSend[2] = 0x80;
            byteListToSend[3] = 0x00;
            byteListToSend[4] = 0x20;

            // remplissage payload
            byte[] byteList = new byte[20];
            for (int i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);

            }
            ////

            int idx = 5;
            foreach (byte b in byteList)
            {
                byteListToSend[idx] = b;
                idx++;
            }
            byteListToSend[idx] = CalculateChecksum(80, 20, byteList);
            sendMessage(byteListToSend);
        }
        public void textBox_Emission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                sendMessage();
                textBox_Emission.Text = "";
            }
        }

        public void BoutonClearboxReception(object sender, RoutedEventArgs e)
        {
            textBox_Reception.Text = "";
        }

        #endregion

        #region Fcnt envoie message
        public void sendMessage()
        {
            string emission = textBox_Emission.Text;
            serialPort1.WriteLine(emission);
        }

        public void sendMessage(byte[] msg)
        {
            serialPort1.Write(msg, 0, msg.Length);
        }
        #endregion

        public byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0xFE;
            checksum ^= (byte)msgFunction;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)msgPayloadLength;
            checksum ^= (byte)(msgPayloadLength >> 8);
            foreach (byte b in msgPayload)
            {
                checksum ^= b;
            }
            return checksum;
        }

        public void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] tableau = new byte[1 + 2 + 2 + msgPayloadLength + 1]; // voir tableau sur TP page22
            int pos = 0;

            tableau[pos++] = 0xFE;
            tableau[pos++] = (byte)(msgFunction >> 8);
            tableau[pos++] = (byte)msgFunction;
            tableau[pos++] = (byte)(msgPayloadLength >> 8);
            tableau[pos++] = (byte)(msgPayloadLength);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                tableau[pos++] = msgPayload[i];
            }
            tableau[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);

            sendMessage(tableau);
        }


        //partie décodage des messages(3.1.2)
        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }


        StateReception rcvState = StateReception.Waiting;
        int calculatedChecksum;
        int receivedChecksum;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;
        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                    {
                        rcvState = StateReception.FunctionMSB;
                    }
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;

                    break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += (int)c;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = (int)c;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength = (int)msgDecodedPayloadLength << 8;
                    msgDecodedPayloadLength += (int)c;

                    msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    msgDecodedPayloadIndex = 0;
                    rcvState = StateReception.Payload;
                    break;
                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex++] = c;

                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    {
                        rcvState = StateReception.CheckSum;
                    }

                    break;
                case StateReception.CheckSum:
                    calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    receivedChecksum = c;
                    if (calculatedChecksum == receivedChecksum)
                    {
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        textBox_Reception.Text += "Error 404";
                        rcvState = StateReception.Waiting;
                    }
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }


        public enum Description
        {
            rien,
            Transmission,
            Led,
            IR,
            vitesse

        }
       

        public void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {

            switch (msgFunction)
            {
                  case 0x0080://Description Transmission
                    textBox_Reception.Text = msgPayload.ToString();
                    //etat = Description.rien;

                    break;
                case 0x0020://Description LED
                    int numled = msgPayload[0];
                    int stateled = msgPayload[1];
                    //etat = Description.rien;
                    break;
                case 0x0030://Description telemetre IR Droite 
                    int telemetreD = (int)msgPayload[0];
                    LabelirD.Content = telemetreD.ToString();

                    //etat = Description.rien;
                    break;
                case 0x0031://Description telemetre IR Centre
                    int telemetreC = msgPayload[0];
                    LabelirC.Content = telemetreC.ToString();
                    //etat = Description.rien;
                    break;
                case 0x0032://Description telemetre IR Gauche
                    int telemetreG = msgPayload[0];
                    LabelirG.Content = telemetreG.ToString();
                    //etat = Description.rien;
                    break;
                case 0x0040://Description Vitesse

                    int vitesseD = msgPayload[1];
                    int vitesseG = msgPayload[0];
                    //etat = Description.rien;
                    break;
            }


        }





    }
}
