package 
{
	import app.core.action.RotatableScalable;
	import flash.display.*;
	import flash.events.*;
	import flash.net.*;
	import flash.geom.*;
	
	[SWF(width="1280", height="800", frameRate="30", backgroundColor="#FFFFFF")]
	
	public class Main extends Sprite 
	{
		private var _width:Number = 1280;
		private var _height:Number = 800;
		
		private var spriteList:Object = new Object();
		
		public function Main() 
		{
		
			//--------connect to TUIO-----------------
			TUIO.init(this,'localhost',3000,'',false);
			trace("MyTouchApp Initialized");
			//----------------------------------------
			
			stage.displayState = StageDisplayState.FULL_SCREEN;
			var background:Sprite = new Sprite();
			background.graphics.beginFill(Math.random*0xFFFFFF,0.5);
			background.graphics.drawRoundRect(0, 0,_width, _height, 15);
			background.graphics.endFill();
			addChild(background);
			
			addEventListener(TouchEvent.MOUSE_DOWN, touchDown); //run touchdown, when touched
			addEventListener(TouchEvent.MOUSE_MOVE, touchMove);
			addEventListener(TouchEvent.MOUSE_UP, touchUp);
		}
		
		public function touchDown(e:TouchEvent):void
		{	
			if (e.ID <= 200) // Its an Object
			{
				
				var curPt:Point = parent.globalToLocal(new Point(e.stageX, e.stageY)); //convert touch points to x,y
				
				var circle:RotatableScalable = new RotatableScalable(); //create a new sprite
				
				circle.graphics.lineStyle(10, 0xff0000); //set line width to 10px and red
				circle.graphics.drawCircle(0,0,100); // draw a 100px circle
				circle.x = curPt.x; //put it where touch is (x cord)
				circle.y = curPt.y; //put it where touch is (y cord)
				
				addChild(circle); //add the circle where touch happened
			}
			else // It is a finger
			{
				var curPt:Point = parent.globalToLocal(new Point(e.stageX, e.stageY)); //convert touch points to x,y
				
				var circle:RotatableScalable = new RotatableScalable(); //create a new sprite
				
				circle.graphics.lineStyle(10, 0xff0000); //set line width to 10px and red
				circle.graphics.drawCircle(0,0,40); // draw a 40px circle
				circle.x = curPt.x; //put it where touch is (x cord)
				circle.y = curPt.y; //put it where touch is (y cord)
				
				addChild(circle); //add the circle where touch happened
			}
			
			spriteList[e.ID] = circle;
		}
		
		public function touchMove(e:TouchEvent):void
		{
			spriteList[e.ID].x = e.stageX;
			spriteList[e.ID].y = e.stageY;
		}
		
		public function touchUp(e:TouchEvent):void
		{
			removeChild(spriteList[e.ID]);
		}
	}
}
