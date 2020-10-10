package example;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Button b;
		super.onCreate(savedInstanceState);
		b=new Button(this);
		b.setOnClickListener(
			new View.OnClickListener() {
				public void onClick(View v) {
					Toast.makeText(MainActivity.this, "Button Clicked", Toast.LENGTH_SHORT).show();
				}
			}
		);
		setContentView(b);
	}
}
