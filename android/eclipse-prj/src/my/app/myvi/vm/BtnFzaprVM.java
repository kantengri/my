package my.app.myvi.vm;

import gueei.binding.Command;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import android.view.View;

public class BtnFzaprVM {	

	private final Logger log = LoggerFactory.getLogger(BtnFzaprVM.class);
	
	public Command Click = new Command(){
		@Override
		public void Invoke(View arg0, Object... arg1) {
			log.debug("fzapr btn clicked !");
			

		}
	};
	
//	public StringObservable midText = new StringObservable("C");
//	public StringObservable botText = new StringObservable("СТАРТ");
}
