namespace CPPBasedCSharpProject
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
            this.ShowImgBtn = new System.Windows.Forms.Button();
            this.FileNameTxtBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // ShowImgBtn
            // 
            this.ShowImgBtn.Location = new System.Drawing.Point(52, 190);
            this.ShowImgBtn.Name = "ShowImgBtn";
            this.ShowImgBtn.Size = new System.Drawing.Size(102, 33);
            this.ShowImgBtn.TabIndex = 0;
            this.ShowImgBtn.Text = "Show Image";
            this.ShowImgBtn.UseVisualStyleBackColor = true;
            this.ShowImgBtn.Click += new System.EventHandler(this.ShowImgBtn_Click);
            // 
            // FileNameTxtBox
            // 
            this.FileNameTxtBox.Location = new System.Drawing.Point(52, 129);
            this.FileNameTxtBox.Name = "FileNameTxtBox";
            this.FileNameTxtBox.Size = new System.Drawing.Size(100, 20);
            this.FileNameTxtBox.TabIndex = 1;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.FileNameTxtBox);
            this.Controls.Add(this.ShowImgBtn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ShowImgBtn;
        private System.Windows.Forms.TextBox FileNameTxtBox;
    }
}

