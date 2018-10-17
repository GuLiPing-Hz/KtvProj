package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.TextView;

/**
 * 加载提示Dialog
 * 
 * @author glp
 */
public class LoadingDialog extends AlertDialog {

    private TextView mUiText;
    private String mMessage = null;

    public LoadingDialog(Context context) {
        super(context);
        mMessage = getContext().getResources().getString(R.string.msg_load_ing);
    }

    public LoadingDialog(Context context, String message) {
        super(context);
        this.mMessage = message;
        this.setCancelable(false);
    }

    public LoadingDialog(Context context, int theme, String message) {
        super(context, theme);
        this.mMessage = message;
        this.setCancelable(false);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(R.layout.view_tips_loading);
        mUiText = (TextView) findViewById(R.id.tips_loading_msg);
        setText(mMessage);
    }

    public void setText(String message) {
        this.mMessage = message;
        mUiText.setText(this.mMessage);
        mUiText.setVisibility(TextUtils.isEmpty(message)?View.GONE:View.VISIBLE);
    }

    public void setText(int resId) {
        setText(getContext().getResources().getString(resId));
    }

}
