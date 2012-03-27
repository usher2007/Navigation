namespace ImageSegmentationDemo
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBoxCT = new System.Windows.Forms.PictureBox();
            this.buttonOpen = new System.Windows.Forms.Button();
            this.buttonHelp = new System.Windows.Forms.Button();
            this.buttonSave = new System.Windows.Forms.Button();
            this.buttonFind = new System.Windows.Forms.Button();
            this.buttonPreprocess = new System.Windows.Forms.Button();
            this.buttonFindTumor = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCT)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxCT
            // 
            this.pictureBoxCT.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.pictureBoxCT.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBoxCT.Cursor = System.Windows.Forms.Cursors.Cross;
            this.pictureBoxCT.ImageLocation = "C:\\Users\\usher\\Pictures\\021A3DBB.bmp";
            this.pictureBoxCT.InitialImage = null;
            this.pictureBoxCT.Location = new System.Drawing.Point(12, 12);
            this.pictureBoxCT.Name = "pictureBoxCT";
            this.pictureBoxCT.Size = new System.Drawing.Size(512, 512);
            this.pictureBoxCT.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxCT.TabIndex = 0;
            this.pictureBoxCT.TabStop = false;
            this.pictureBoxCT.LoadCompleted += new System.ComponentModel.AsyncCompletedEventHandler(this.pictureBoxCT_LoadCompleted);
            this.pictureBoxCT.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBoxCT_MouseDown);
            this.pictureBoxCT.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBoxCT_MouseMove);
            this.pictureBoxCT.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBoxCT_MouseUp);
            // 
            // buttonOpen
            // 
            this.buttonOpen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOpen.Location = new System.Drawing.Point(533, 12);
            this.buttonOpen.Name = "buttonOpen";
            this.buttonOpen.Size = new System.Drawing.Size(94, 33);
            this.buttonOpen.TabIndex = 1;
            this.buttonOpen.Text = "Open File";
            this.buttonOpen.UseVisualStyleBackColor = true;
            this.buttonOpen.Click += new System.EventHandler(this.buttonOpen_Click);
            // 
            // buttonHelp
            // 
            this.buttonHelp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonHelp.Location = new System.Drawing.Point(533, 70);
            this.buttonHelp.Name = "buttonHelp";
            this.buttonHelp.Size = new System.Drawing.Size(94, 33);
            this.buttonHelp.TabIndex = 2;
            this.buttonHelp.Text = "Help";
            this.buttonHelp.UseVisualStyleBackColor = true;
            this.buttonHelp.Click += new System.EventHandler(this.buttonHelp_Click);
            // 
            // buttonSave
            // 
            this.buttonSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSave.Location = new System.Drawing.Point(533, 491);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(94, 33);
            this.buttonSave.TabIndex = 3;
            this.buttonSave.Text = "Save As";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // buttonFind
            // 
            this.buttonFind.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonFind.Location = new System.Drawing.Point(533, 325);
            this.buttonFind.Name = "buttonFind";
            this.buttonFind.Size = new System.Drawing.Size(94, 33);
            this.buttonFind.TabIndex = 4;
            this.buttonFind.Text = "Find Bone";
            this.buttonFind.UseVisualStyleBackColor = true;
            this.buttonFind.Click += new System.EventHandler(this.buttonFind_Click);
            // 
            // buttonPreprocess
            // 
            this.buttonPreprocess.Location = new System.Drawing.Point(533, 381);
            this.buttonPreprocess.Name = "buttonPreprocess";
            this.buttonPreprocess.Size = new System.Drawing.Size(94, 33);
            this.buttonPreprocess.TabIndex = 5;
            this.buttonPreprocess.Text = "Preprocess";
            this.buttonPreprocess.UseVisualStyleBackColor = true;
            this.buttonPreprocess.Click += new System.EventHandler(this.buttonPreprocess_Click);
            // 
            // buttonFindTumor
            // 
            this.buttonFindTumor.Location = new System.Drawing.Point(533, 439);
            this.buttonFindTumor.Name = "buttonFindTumor";
            this.buttonFindTumor.Size = new System.Drawing.Size(94, 33);
            this.buttonFindTumor.TabIndex = 6;
            this.buttonFindTumor.Text = "Find Tumor";
            this.buttonFindTumor.UseVisualStyleBackColor = true;
            this.buttonFindTumor.Click += new System.EventHandler(this.buttonFindTumor_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(639, 541);
            this.Controls.Add(this.buttonFindTumor);
            this.Controls.Add(this.buttonPreprocess);
            this.Controls.Add(this.buttonFind);
            this.Controls.Add(this.buttonSave);
            this.Controls.Add(this.buttonHelp);
            this.Controls.Add(this.buttonOpen);
            this.Controls.Add(this.pictureBoxCT);
            this.MaximumSize = new System.Drawing.Size(655, 579);
            this.MinimumSize = new System.Drawing.Size(655, 579);
            this.Name = "Form1";
            this.Text = "Image Segment Demo";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxCT)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxCT;
        private System.Windows.Forms.Button buttonOpen;
        private System.Windows.Forms.Button buttonHelp;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Button buttonFind;
        private System.Windows.Forms.Button buttonPreprocess;
        private System.Windows.Forms.Button buttonFindTumor;
    }
}

