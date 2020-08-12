package com.example.koncisepeda;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class Seting extends AppCompatActivity {
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference myRef = database.getReference();

    FirebaseAuth firebaseAuth;
    FirebaseUser firebaseUser;

    final DatabaseReference seting_hidup = myRef.child("seting").child("hidup");
    final DatabaseReference seting_mati = myRef.child("seting").child("mati");

    Button simpan;
    EditText setinghidup, setingmati;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_seting);
        setinghidup = (EditText) findViewById(R.id.seting_hidup);
        setingmati = (EditText) findViewById(R.id.seting_mati);
        simpan = (Button) findViewById(R.id.btnsimpankondisi);

        seting_hidup.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                String value = dataSnapshot.getValue(String.class);
                Log.d("file", "Value is: " + value);
                String status = value;
                setinghidup.setText(status );
            }

            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value
                Log.w("file", "Failed to read value.", error.toException());
            }
        });
        seting_mati.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                String value = dataSnapshot.getValue(String.class);
                Log.d("file", "Value is: " + value);
                String status = value;
                setingmati.setText(status );
            }

            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value
                Log.w("file", "Failed to read value.", error.toException());
            }
        });

        simpan.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){

                FirebaseDatabase database = FirebaseDatabase.getInstance();
                DatabaseReference myRef = database.getReference("seting/hidup");
                myRef.setValue(setinghidup.getText().toString());
                myRef = database.getReference("seting/mati");
                myRef.setValue(setingmati.getText().toString());
                Toast.makeText(Seting.this, "Simpan seting berhasil", Toast.LENGTH_LONG).show();
                Intent intent = new Intent(Seting.this, MainActivity.class);
                startActivity(intent);
                finish();
            }
        });
    }
}
