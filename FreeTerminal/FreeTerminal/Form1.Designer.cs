namespace FreeTerminal
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.addressBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.openPort = new System.Windows.Forms.Button();
            this.getAddress = new System.Windows.Forms.Button();
            this.resetZero = new System.Windows.Forms.Button();
            this.closePort = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.SuspendLayout();
            // 
            // addressBox
            // 
            this.addressBox.Location = new System.Drawing.Point(12, 30);
            this.addressBox.Name = "addressBox";
            this.addressBox.ReadOnly = true;
            this.addressBox.Size = new System.Drawing.Size(100, 20);
            this.addressBox.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(83, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Адрес привода";
            // 
            // openPort
            // 
            this.openPort.Location = new System.Drawing.Point(15, 111);
            this.openPort.Name = "openPort";
            this.openPort.Size = new System.Drawing.Size(111, 24);
            this.openPort.TabIndex = 2;
            this.openPort.Text = "Открыть порт";
            this.openPort.UseVisualStyleBackColor = true;
            this.openPort.Click += new System.EventHandler(this.openPort_Click);
            // 
            // getAddress
            // 
            this.getAddress.Enabled = false;
            this.getAddress.Location = new System.Drawing.Point(161, 30);
            this.getAddress.Name = "getAddress";
            this.getAddress.Size = new System.Drawing.Size(111, 23);
            this.getAddress.TabIndex = 3;
            this.getAddress.Text = "Отобразить адрес";
            this.getAddress.UseVisualStyleBackColor = true;
            this.getAddress.Click += new System.EventHandler(this.getAddress_Click);
            // 
            // resetZero
            // 
            this.resetZero.Enabled = false;
            this.resetZero.Location = new System.Drawing.Point(161, 69);
            this.resetZero.Name = "resetZero";
            this.resetZero.Size = new System.Drawing.Size(111, 23);
            this.resetZero.TabIndex = 4;
            this.resetZero.Text = "Сбросить нуль";
            this.resetZero.UseVisualStyleBackColor = true;
            this.resetZero.Click += new System.EventHandler(this.resetZero_Click);
            // 
            // closePort
            // 
            this.closePort.Enabled = false;
            this.closePort.Location = new System.Drawing.Point(161, 111);
            this.closePort.Name = "closePort";
            this.closePort.Size = new System.Drawing.Size(111, 23);
            this.closePort.TabIndex = 5;
            this.closePort.Text = "Закрыть порт";
            this.closePort.UseVisualStyleBackColor = true;
            this.closePort.Click += new System.EventHandler(this.closePort_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 2400;
            this.serialPort1.PortName = "COM3";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 146);
            this.Controls.Add(this.closePort);
            this.Controls.Add(this.resetZero);
            this.Controls.Add(this.getAddress);
            this.Controls.Add(this.openPort);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.addressBox);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Free Terminal";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox addressBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button openPort;
        private System.Windows.Forms.Button getAddress;
        private System.Windows.Forms.Button resetZero;
        private System.Windows.Forms.Button closePort;
        private System.IO.Ports.SerialPort serialPort1;
    }
}

