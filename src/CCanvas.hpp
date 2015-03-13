class Canvas {
	public:
		Canvas();
		~Canvas();
		
		void SetSize(const unsigned int width, const unsigned int height);
		void GetSize(unsigned int& width, unsigned int& height);
		
		// Coordinate Space Context
		void Push();
		void Pop();
		void Translate(const unsigned int x, const unsigned int y);
		void Rotate(const float radians);
		void Scale(const unsigned int sx, const unsigned int sy);
		
		// Drawing
		void Draw(const TextBuffer buffer, const unsigned int x, const unsigned int y);
		
		// Maintenance
		void Clear();
		void Swap();
		
		TextBuffer& GetBuffer();
		
	private:
		int bufferID;
		TextBuffer buffers[2];
		
}
